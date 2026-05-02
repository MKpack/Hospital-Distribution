#include "voronoi_calculator.h"
#include <QLineF>
#include <QRectF>
#include <cmath>
#include <algorithm>
#include <numeric>
#include <limits>

VoronoiCalculator::VoronoiCalculator()
    : m_reasonableDistance(1000.0) {
}

void VoronoiCalculator::setHospitals(const QVector<Hospital>& hospitals) {
    m_hospitals = hospitals;
}

void VoronoiCalculator::setBoundary(const QRectF& boundary) {
    m_boundary = boundary;
}

QVector<VoronoiCell> VoronoiCalculator::computeVoronoi() {
    QVector<VoronoiCell> cells;
    if (m_hospitals.size() < 2) {
        for (int i = 0; i < m_hospitals.size(); ++i) {
            VoronoiCell cell;
            cell.hospitalId = m_hospitals[i].id;
            cell.vertices.append(m_boundary.topLeft());
            cell.vertices.append(m_boundary.topRight());
            cell.vertices.append(m_boundary.bottomRight());
            cell.vertices.append(m_boundary.bottomLeft());
            cell.centroid = computeCentroid(cell.vertices);
            cell.area = computeArea(cell.vertices);
            cell.isBounded = true;
            cells.append(cell);
        }
        return cells;
    }

    int gridSize = 50;
    double xStep = m_boundary.width() / gridSize;
    double yStep = m_boundary.height() / gridSize;

    QMap<int, QVector<QPointF>> cellPoints;

    for (double x = m_boundary.left(); x <= m_boundary.right(); x += xStep) {
        for (double y = m_boundary.top(); y <= m_boundary.bottom(); y += yStep) {
            QPointF point(x, y);
            double minDist = std::numeric_limits<double>::max();
            int nearestHospital = -1;

            for (int i = 0; i < m_hospitals.size(); ++i) {
                double dist = QLineF(point, m_hospitals[i].position).length();
                if (dist < minDist) {
                    minDist = dist;
                    nearestHospital = i;
                }
            }

            if (nearestHospital >= 0) {
                cellPoints[nearestHospital].append(point);
            }
        }
    }

    for (int i = 0; i < m_hospitals.size(); ++i) {
        VoronoiCell cell;
        cell.hospitalId = m_hospitals[i].id;

        if (cellPoints.contains(i) && !cellPoints[i].isEmpty()) {
            double min_x = std::numeric_limits<double>::max();
            double max_x = std::numeric_limits<double>::lowest();
            double min_y = std::numeric_limits<double>::max();
            double max_y = std::numeric_limits<double>::lowest();

            for (const QPointF& p : cellPoints[i]) {
                min_x = std::min(min_x, p.x());
                max_x = std::max(max_x, p.x());
                min_y = std::min(min_y, p.y());
                max_y = std::max(max_y, p.y());
            }

            cell.vertices.append(QPointF(min_x, min_y));
            cell.vertices.append(QPointF(max_x, min_y));
            cell.vertices.append(QPointF(max_x, max_y));
            cell.vertices.append(QPointF(min_x, max_y));
            cell.isBounded = true;
        } else {
            cell.vertices.append(m_boundary.topLeft());
            cell.vertices.append(m_boundary.topRight());
            cell.vertices.append(m_boundary.bottomRight());
            cell.vertices.append(m_boundary.bottomLeft());
            cell.isBounded = true;
        }

        cell.centroid = computeCentroid(cell.vertices);
        cell.area = computeArea(cell.vertices);
        cells.append(cell);
    }

    return cells;
}

CityEvaluationResult VoronoiCalculator::evaluate(const QVector<VoronoiCell>& cells) {
    CityEvaluationResult result;
    result.totalHospitals = m_hospitals.size();
    result.reasonableCount = 0;
    result.unreasonableCount = 0;

    double totalAvgDist = 0;
    for (int i = 0; i < cells.size(); ++i) {
        HospitalEvaluation eval;
        int hospitalIdx = -1;
        for (int j = 0; j < m_hospitals.size(); ++j) {
            if (m_hospitals[j].id == cells[i].hospitalId) {
                hospitalIdx = j;
                break;
            }
        }

        if (hospitalIdx >= 0) {
            eval.hospitalId = m_hospitals[hospitalIdx].id;
            eval.hospitalName = m_hospitals[hospitalIdx].name;
            eval.maxDistance = computeMaxDistance(m_hospitals[hospitalIdx].position, cells[i].vertices);
            eval.avgDistance = eval.maxDistance * 0.5;
            eval.isReasonable = eval.maxDistance <= m_reasonableDistance;

            if (eval.isReasonable) {
                result.reasonableCount++;
            } else {
                result.unreasonableCount++;
            }

            totalAvgDist += eval.avgDistance;
            result.globalMaxDistance = std::max(result.globalMaxDistance, eval.maxDistance);
        }

        result.hospitalEvaluations.append(eval);
    }

    if (result.totalHospitals > 0) {
        result.globalAvgDistance = totalAvgDist / result.totalHospitals;
    }
    result.overallCoverage = (result.totalHospitals > 0) ?
        (static_cast<double>(result.reasonableCount) / result.totalHospitals * 100.0) : 0;
    result.isOverallReasonable = (result.unreasonableCount == 0);

    return result;
}

QPointF VoronoiCalculator::computeCentroid(const QVector<QPointF>& vertices) {
    double cx = 0, cy = 0;
    for (const QPointF& v : vertices) {
        cx += v.x();
        cy += v.y();
    }
    if (!vertices.isEmpty()) {
        cx /= vertices.size();
        cy /= vertices.size();
    }
    return QPointF(cx, cy);
}

double VoronoiCalculator::computeArea(const QVector<QPointF>& vertices) {
    if (vertices.size() < 3) return 0;
    double area = 0;
    int n = vertices.size();
    for (int i = 0; i < n; ++i) {
        int j = (i + 1) % n;
        area += vertices[i].x() * vertices[j].y();
        area -= vertices[j].x() * vertices[i].y();
    }
    return std::abs(area) / 2.0;
}

double VoronoiCalculator::computeMaxDistance(const QPointF& hospital, const QVector<QPointF>& vertices) {
    double maxDist = 0;
    for (const QPointF& v : vertices) {
        double dist = QLineF(hospital, v).length();
        maxDist = std::max(maxDist, dist);
    }
    return maxDist;
}

double VoronoiCalculator::computeAvgDistance(const QPointF& hospital, const QVector<VoronoiCell>& cells) {
    return 0;
}

QVector<QPointF> VoronoiCalculator::clipToBoundary(const QVector<QPointF>& vertices, const QRectF& boundary) {
    return vertices;
}
