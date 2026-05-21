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

    // 调用网页端评分函数，计算当前医院分布的合理性。
    void calculateRationality();

    // 根据行政区或地点名称搜索医院，并在地图上展示结果。
    void searchHosptial(const QString &region);

    // 根据当前医院点位生成 Voronoi 区域划分。
    void runVoronoiAnalysis();

    // 从网页端异步读取评分结果，读取完成后通过回调返回给主窗口。
    void getScoreResult(std::function<void(const QString&)> callback);
private:
    // 内嵌百度地图网页的浏览器控件。
    QWebEngineView *m_view;

signals:
};

#endif // MAPCONTROLLER_H
