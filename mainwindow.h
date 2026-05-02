#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "model/hospital.h"
#include "model/voronoi_cell.h"
#include "model/evaluation_result.h"
#include "voronoi/voronoi_calculator.h"

namespace Ui {
class MainWindow;
}

class VoronoiScene;
class VoronoiView;
class HospitalInputWidget;
class ResultPanel;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onEvaluateClicked();
    void onHospitalsChanged(const QVector<Hospital>& hospitals);

private:
    Ui::MainWindow *ui;
    VoronoiScene* m_scene;
    VoronoiView* m_view;
    HospitalInputWidget* m_inputWidget;
    ResultPanel* m_resultPanel;
    VoronoiCalculator m_calculator;
    QVector<Hospital> m_hospitals;
    QRectF m_boundary;

    void setupUI();
    void updateView();
};

#endif
