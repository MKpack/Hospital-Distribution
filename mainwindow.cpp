#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 将地图网页嵌入 ui->mapWidget，后续按钮操作都通过该控制器转发给 JS。
    mapController = new MapController(ui->mapWidget, this);

    // 点击“搜索”后读取输入框内容，并在地图中检索对应区域的医院。
    connect(ui->searchBtn, &QPushButton::clicked, this, [this]() {
        QString region = ui->searchEdit->text().trimmed();
        if (!region.isEmpty()) {
            mapController->searchHosptial(region);
        }
    });

    // 支持在输入框中按回车直接触发搜索按钮，减少重复代码。
    connect(ui->searchEdit, &QLineEdit::returnPressed, ui->searchBtn, &QPushButton::click);

    // 生成医院点位的 Voronoi 服务区，用于观察区域划分。
    connect(ui->voronoiBtn, &QPushButton::clicked, this, [this]() {
        mapController->runVoronoiAnalysis();
    });

    // 触发网页端的合理性评分计算，再异步取回 JS 写入的综合分数。
    connect(ui->calculateBtn, &QPushButton::clicked, this, [this]() {
        mapController->calculateRationality();
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
