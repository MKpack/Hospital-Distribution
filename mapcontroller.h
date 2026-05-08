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
    void calculateRationality();
    void searchHosptial(const QString &region);
    void runVoronoiAnalysis();
    void getScoreResult(std::function<void(const QString&)> callback);
private:
    QWebEngineView *m_view;

signals:
};

#endif // MAPCONTROLLER_H
