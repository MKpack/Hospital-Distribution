#include "mapcontroller.h"
#include <QLayout>
#include <QUrl>
#include <QWebEnginePage>

MapController::MapController(QWidget *container, QObject *parent)
    : QObject{parent}
{
    m_view = new QWebEngineView(container);
    QLayout *interLayout = container->layout();

    interLayout->addWidget(m_view);
    interLayout->setContentsMargins(0, 0, 0, 0);

    m_view->setUrl(QUrl("qrc:/index.html"));
}

void MapController::getScoreResult(std::function<void(const QString&)> callback)
{
    m_view->page()->runJavaScript("window.jsScoreResult", [callback](const QVariant& result) {
        callback(result.toString());
    });
}

void MapController::searchHosptial(const QString &region)
{
    QString escaped = region;
    escaped.replace("\\", "\\\\").replace("'", "\\'");
    QString js = QString("searchHospitals('%1')").arg(escaped);
    m_view->page()->runJavaScript(js);
}

void MapController::runVoronoiAnalysis()
{
    m_view->page()->runJavaScript("runVoronoiAnalysis()");
}

// ===================== 新增：医院分布合理性计算 =====================
void MapController::calculateRationality()
{
    m_view->page()->runJavaScript("calculateRationality()");
}