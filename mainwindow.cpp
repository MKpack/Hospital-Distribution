#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    mapController = new MapController(ui->mapWidget, this);

    connect(ui->searchBtn, &QPushButton::clicked, this, [this]() {
        QString region = ui->searchEdit->text().trimmed();
        if (!region.isEmpty()) {
            mapController->searchHosptial(region);
        }
    });

    connect(ui->searchEdit, &QLineEdit::returnPressed, ui->searchBtn, &QPushButton::click);

    connect(ui->voronoiBtn, &QPushButton::clicked, this, [this]() {
        mapController->runVoronoiAnalysis();
    });

    // ===================== 新增：医院分布合理性计算 =====================
    connect(ui->calculateBtn, &QPushButton::clicked, this, [this]() {
        mapController->calculateRationality();
        // 等JS计算完成后，获取结果并显示
        mapController->getScoreResult([this](const QString& score) {
            if (!score.isEmpty()) {
                QMessageBox::information(this, "分析完成",
                                         QString("医院分布合理性计算已完成！\n综合评分：%1 分\n请在地图上查看详细结果。").arg(score));
            }
        });
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}