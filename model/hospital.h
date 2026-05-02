#ifndef HOSPITAL_H
#define HOSPITAL_H

#include <QString>
#include <QPointF>

struct Hospital {
    int id;
    QString name;
    QPointF position;

    Hospital() : id(0), position(0, 0) {}
    Hospital(int id, const QString& name, double x, double y)
        : id(id), name(name), position(x, y) {}
    Hospital(int id, const QString& name, const QPointF& pos)
        : id(id), name(name), position(pos) {}
};

#endif
