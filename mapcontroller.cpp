#include "mapcontroller.h"
#include <QLayout>
#include <QUrl>
#include <QWebEnginePage>

MapController::MapController(QWidget *container, QObject *parent)
    : QObject{parent}
{
    // 在主窗口预留的 QWidget 容器中创建浏览器视图。
    m_view = new QWebEngineView(container);
    QLayout *interLayout = container->layout();

    // 让网页地图铺满容器，避免四周出现默认边距。
    interLayout->addWidget(m_view);
    interLayout->setContentsMargins(0, 0, 0, 0);

    // index.html 通过 res.qrc 打包进 Qt 资源，运行时用 qrc 路径加载。
    m_view->setUrl(QUrl("qrc:/index.html"));
}

void MapController::getScoreResult(std::function<void(const QString&)> callback)
{
    // runJavaScript 是异步接口，JS 执行结果会在 lambda 中返回。
    m_view->page()->runJavaScript("window.jsScoreResult", [callback](const QVariant& result) {
        callback(result.toString());
    });
}

void MapController::searchHosptial(const QString &region)
{
    // 将用户输入转义后拼接进 JS 字符串，避免反斜杠或单引号破坏脚本。
    QString escaped = region;
    escaped.replace("\\", "\\\\").replace("'", "\\'");
    QString js = QString("searchHospitals('%1')").arg(escaped);
    m_view->page()->runJavaScript(js);
}

void MapController::runVoronoiAnalysis()
{
    // 具体算法在网页端实现，C++ 只负责触发。
    m_view->page()->runJavaScript("runVoronoiAnalysis()");
}

void MapController::calculateRationality()
{
    // 评分逻辑依赖网页中已经加载的医院数组 hospitalData。
    m_view->page()->runJavaScript("calculateRationality()");
}
