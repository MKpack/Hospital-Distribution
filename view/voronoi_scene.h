#ifndef VORONOI_SCENE_H
#define VORONOI_SCENE_H

#include <QGraphicsScene>
#include <QVector>
#include "../model/hospital.h"
#include "../model/voronoi_cell.h"
#include "../model/evaluation_result.h"

class VoronoiScene : public QGraphicsScene {
    Q_OBJECT

public:
    VoronoiScene(QObject* parent = nullptr);

    void setHospitals(const QVector<Hospital>& hospitals);
    void setCells(const QVector<VoronoiCell>& cells);
    void setEvaluation(const CityEvaluationResult& eval);
    void clear();

    void renderVoronoi();
    void renderHospitals();

signals:
    void cellHovered(int hospitalId, const QPointF& pos);

private:
    QVector<Hospital> m_hospitals;
    QVector<VoronoiCell> m_cells;
    CityEvaluationResult m_evaluation;

    QColor getCellColor(const HospitalEvaluation& eval);
};

#endif
