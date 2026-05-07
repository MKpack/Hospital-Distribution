#include "mapcontroller.h"
#include <QLayout>
#include <QUrl>

MapController::MapController(QWidget *container, QObject *parent)
    : QObject{parent}
{
    m_view = new QWebEngineView(container);
    QLayout *interLayout = container->layout();

    interLayout->addWidget(m_view);
    interLayout->setContentsMargins(0, 0, 0, 0);

    m_view->setUrl(QUrl("qrc:/index.html"));
}

