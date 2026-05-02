#include "voronoi_scene.h"
#include <QGraphicsPolygonItem>
#include <QGraphicsEllipseItem>
#include <QGraphicsTextItem>
#include <QPen>
#include <QBrush>
#include <QFont>

VoronoiScene::VoronoiScene(QObject* parent)
    : QGraphicsScene(parent) {
}

void VoronoiScene::setHospitals(const QVector<Hospital>& hospitals) {
    m_hospitals = hospitals;
}

void VoronoiScene::setCells(const QVector<VoronoiCell>& cells) {
    m_cells = cells;
}

void VoronoiScene::setEvaluation(const CityEvaluationResult& eval) {
    m_evaluation = eval;
}

void VoronoiScene::clear() {
    clear();
    QGraphicsScene::clear();
}

void VoronoiScene::renderVoronoi() {
    QGraphicsScene::clear();

    for (const VoronoiCell& cell : m_cells) {
        if (cell.vertices.size() < 3) continue;

        QPolygonF polygon;
        for (const QPointF& v : cell.vertices) {
            polygon.append(v);
        }

        QGraphicsPolygonItem* item = addPolygon(polygon);
        item->setPen(QPen(Qt::black, 1, Qt::SolidLine));

        int hospitalIdx = -1;
        for (int i = 0; i < m_hospitals.size(); ++i) {
            if (m_hospitals[i].id == cell.hospitalId) {
                hospitalIdx = i;
                break;
            }
        }

        QColor fillColor;
        if (hospitalIdx >= 0 && hospitalIdx < m_evaluation.hospitalEvaluations.size()) {
            fillColor = getCellColor(m_evaluation.hospitalEvaluations[hospitalIdx]);
        } else {
            fillColor = QColor(200, 200, 200, 100);
        }
        item->setBrush(QBrush(fillColor));

        if (!cell.vertices.isEmpty()) {
            QString label = QString("H%1").arg(cell.hospitalId);
            QGraphicsTextItem* textItem = addText(label);
            textItem->setPos(cell.centroid.x() - 10, cell.centroid.y() - 10);
            textItem->setDefaultTextColor(Qt::black);
            textItem->setFont(QFont("Arial", 8));
        }
    }
}

void VoronoiScene::renderHospitals() {
    for (const Hospital& h : m_hospitals) {
        QGraphicsEllipseItem* dot = addEllipse(
            h.position.x() - 4, h.position.y() - 4, 8, 8);
        dot->setBrush(QBrush(Qt::red));
        dot->setPen(QPen(Qt::black, 1));
        dot->setZValue(10);

        QGraphicsTextItem* label = addText(h.name);
        label->setPos(h.position.x() + 6, h.position.y() - 6);
        label->setDefaultTextColor(Qt::black);
        label->setFont(QFont("Arial", 7));
        label->setZValue(11);
    }
}

QColor VoronoiScene::getCellColor(const HospitalEvaluation& eval) {
    if (eval.isReasonable) {
        return QColor(144, 238, 144, 100);
    } else {
        return QColor(255, 182, 193, 100);
    }
}
