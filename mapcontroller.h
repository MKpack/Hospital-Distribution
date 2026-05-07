#ifndef MAPCONTROLLER_H
#define MAPCONTROLLER_H

#include <QObject>
#include <QWebEngineView>
#include <QWebChannel>

class MapController : public QObject
{
    Q_OBJECT
public:
    explicit MapController(QWidget *container,QObject *parent = nullptr);

    void searchHosptial(const QString &region);
    void runVoronoiAnalysis();

private:
    QWebEngineView *m_view;

signals:
};

#endif // MAPCONTROLLER_H
