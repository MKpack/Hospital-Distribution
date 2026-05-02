#ifndef VORONOI_VIEW_H
#define VORONOI_VIEW_H

#include <QGraphicsView>
#include <QMouseEvent>
#include <QWheelEvent>

class VoronoiView : public QGraphicsView {
    Q_OBJECT

public:
    VoronoiView(QWidget* parent = nullptr);

protected:
    void wheelEvent(QWheelEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;

private:
    bool m_isPanning;
    QPoint m_lastMousePos;
};

#endif
