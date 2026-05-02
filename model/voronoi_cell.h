#ifndef VORONOICELL_H
#define VORONOICELL_H

#include <QVector>
#include <QPointF>

struct VoronoiCell {
    int hospitalId;
    QVector<QPointF> vertices;
    QPointF centroid;
    double area;
    bool isBounded;

    VoronoiCell()
        : hospitalId(0), area(0), isBounded(true) {}
};

#endif
