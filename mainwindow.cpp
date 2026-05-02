#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "view/voronoi_scene.h"
#include "view/voronoi_view.h"
#include "widget/hospital_input_widget.h"
#include "widget/result_panel.h"
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_scene(nullptr)
    , m_view(nullptr)
    , m_inputWidget(nullptr)
    , m_resultPanel(nullptr)
    , m_boundary(0, 0, 10000, 10000) {
    ui->setupUi(this);
    setupUI();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::setupUI() {
    m_scene = new VoronoiScene(this);
    m_scene->setSceneRect(m_boundary);

    m_view = new VoronoiView(ui->viewWidget);
    m_view->setScene(m_scene);

    QVBoxLayout* viewLayout = new QVBoxLayout(ui->viewWidget);
    viewLayout->setContentsMargins(0, 0, 0, 0);
    viewLayout->addWidget(m_view);

    m_inputWidget = new HospitalInputWidget(ui->inputWidget);
    QVBoxLayout* inputLayout = new QVBoxLayout(ui->inputWidget);
    inputLayout->setContentsMargins(0, 0, 0, 0);
    inputLayout->addWidget(m_inputWidget);

    m_resultPanel = new ResultPanel(ui->resultWidget);
    QVBoxLayout* resultLayout = new QVBoxLayout(ui->resultWidget);
    resultLayout->setContentsMargins(0, 0, 0, 0);
    resultLayout->addWidget(m_resultPanel);

    connect(ui->evaluateBtn, &QPushButton::clicked, this, &MainWindow::onEvaluateClicked);
    connect(m_inputWidget, &HospitalInputWidget::hospitalsChanged,
            this, &MainWindow::onHospitalsChanged);

    m_inputWidget->addHospital();
    m_inputWidget->addHospital();
    m_inputWidget->addHospital();
    m_inputWidget->setHospitals({
        Hospital(1, "Hospital A", 5000, 5000),
        Hospital(2, "Hospital B", 3000, 3000),
        Hospital(3, "Hospital C", 7000, 7000)
    });
}

void MainWindow::onEvaluateClicked() {
    m_hospitals = m_inputWidget->getHospitals();
    if (m_hospitals.size() < 1) {
        m_resultPanel->clear();
        m_scene->clear();
        return;
    }

    m_calculator.setHospitals(m_hospitals);
    m_calculator.setBoundary(m_boundary);

    QVector<VoronoiCell> cells = m_calculator.computeVoronoi();
    CityEvaluationResult result = m_calculator.evaluate(cells);

    m_scene->setHospitals(m_hospitals);
    m_scene->setCells(cells);
    m_scene->setEvaluation(result);
    m_scene->renderVoronoi();
    m_scene->renderHospitals();

    m_view->fitInView(m_scene->sceneRect(), Qt::KeepAspectRatio);

    m_resultPanel->setEvaluation(result);

    ui->statusbar->showMessage(QString("Evaluation complete: %1 hospitals processed").arg(m_hospitals.size()), 5000);
}

void MainWindow::onHospitalsChanged(const QVector<Hospital>& hospitals) {
    m_hospitals = hospitals;
    updateView();
}

void MainWindow::updateView() {
    if (m_hospitals.isEmpty()) {
        m_scene->clear();
        return;
    }

    m_scene->setHospitals(m_hospitals);
    m_scene->renderHospitals();
}
