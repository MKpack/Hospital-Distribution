#ifndef VORONOI_CALCULATOR_H
#define VORONOI_CALCULATOR_H

#include <QVector>
#include <QPointF>
#include <QRectF>
#include <QMap>
#include "../model/hospital.h"
#include "../model/voronoi_cell.h"
#include "../model/evaluation_result.h"

class VoronoiCalculator {
public:
    VoronoiCalculator();

    void setHospitals(const QVector<Hospital>& hospitals);
    void setBoundary(const QRectF& boundary);

    QVector<VoronoiCell> computeVoronoi();
    CityEvaluationResult evaluate(const QVector<VoronoiCell>& cells);

private:
    QVector<Hospital> m_hospitals;
    QRectF m_boundary;
    double m_reasonableDistance;

    QPointF computeCentroid(const QVector<QPointF>& vertices);
    double computeArea(const QVector<QPointF>& vertices);
    double computeMaxDistance(const QPointF& hospital, const QVector<QPointF>& vertices);
    double computeAvgDistance(const QPointF& hospital, const QVector<VoronoiCell>& cells);
    QVector<QPointF> clipToBoundary(const QVector<QPointF>& vertices, const QRectF& boundary);
};

#endif
