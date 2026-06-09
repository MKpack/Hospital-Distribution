#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QTimer>
#include <memory>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 将地图网页嵌入 ui->mapWidget，后续按钮操作都通过该控制器转发给 JS。
    mapController = new MapController(ui->mapWidget, this);
    resetScorePanel();

    // 后续操作依赖行政区边界，程序启动时先禁用。
    ui->searchBtn->setEnabled(false);
    ui->voronoiBtn->setEnabled(false);
    ui->calculateBtn->setEnabled(false);

    // 输入内容变化后，已有行政区边界不再可信，需要重新点击“获取行政区”。
    connect(ui->searchEdit, &QLineEdit::textChanged, this, [this](const QString& text) {
        if (text.trimmed() != loadedRegion) {
            ui->searchBtn->setEnabled(false);
            ui->voronoiBtn->setEnabled(false);
            ui->calculateBtn->setEnabled(false);
            resetScorePanel();
        }
    });

    // 第一步：点击“获取行政区”后读取边界并显示行政区轮廓。
    connect(ui->regionBtn, &QPushButton::clicked, this, [this]() {
        QString region = ui->searchEdit->text().trimmed();
        if (!region.isEmpty()) {
            loadedRegion = region;
            resetScorePanel();
            mapController->loadRegionBoundary(region);
            ui->searchBtn->setEnabled(true);
            ui->voronoiBtn->setEnabled(false);
            ui->calculateBtn->setEnabled(false);
        }
    });

    // 第二步：点击“搜索医院”后，只在已获取边界的行政区内检索医院。
    connect(ui->searchBtn, &QPushButton::clicked, this, [this]() {
        QString region = ui->searchEdit->text().trimmed();
        if (!region.isEmpty() && region == loadedRegion) {
            resetScorePanel();
            mapController->searchHosptial(region);
            ui->searchBtn->setEnabled(false);
            ui->voronoiBtn->setEnabled(false);
            ui->calculateBtn->setEnabled(false);

            QTimer *timer = new QTimer(this);
            auto attempts = std::make_shared<int>(0);
            connect(timer, &QTimer::timeout, this, [this, timer, attempts]() {
                (*attempts)++;
                mapController->getHospitalSearchState([this, timer, attempts](bool finished, int hospitalCount) {
                    if (!finished && *attempts < 60)
                        return;

                    timer->stop();
                    timer->deleteLater();
                    if (ui->searchEdit->text().trimmed() != loadedRegion)
                        return;

                    ui->searchBtn->setEnabled(true);

                    // Voronoi 和合理性评分都至少需要 3 家医院。
                    const bool enoughHospitals = finished && hospitalCount >= 3;
                    ui->voronoiBtn->setEnabled(enoughHospitals);
                    ui->calculateBtn->setEnabled(enoughHospitals);
                });
            });
            timer->start(500);
        }
    });

    // 回车先执行行政区获取，符合“先边界、后医院”的操作顺序。
    connect(ui->searchEdit, &QLineEdit::returnPressed, ui->regionBtn, &QPushButton::click);

    // 生成医院点位的 Voronoi 服务区，用于观察区域划分。
    connect(ui->voronoiBtn, &QPushButton::clicked, this, [this]() {
        mapController->runVoronoiAnalysis();
    });

    // 触发网页端的合理性评分计算，再把结果显示到右侧评分面板。
    connect(ui->calculateBtn, &QPushButton::clicked, this, [this]() {
        updateScorePanel();
    });
}

void MainWindow::resetScorePanel()
{
    ui->scoreValueLabel->setText("-- 分");
    ui->scoreValueLabel->setStyleSheet("color: #1f7a45; font-size: 24px; font-weight: bold;");
    ui->scoreStatusLabel->setText("未计算");
    ui->scoreDetailLabel->setText("点击分布合理性计算后显示。");
    ui->scorePanel->setVisible(false);
}

void MainWindow::showScoreResult(const QString &score)
{
    QJsonParseError error;
    const QJsonDocument document = QJsonDocument::fromJson(score.toUtf8(), &error);
    if (error.error != QJsonParseError::NoError || !document.isObject()) {
        bool ok = false;
        const double value = score.toDouble(&ok);
        if (!ok) {
            resetScorePanel();
            return;
        }

        const bool isReasonable = value >= 60.0;
        const QString color = isReasonable ? "#1f7a45" : "#b3261e";
        ui->scoreValueLabel->setStyleSheet(
            QString("color: %1; font-size: 24px; font-weight: bold;").arg(color));
        ui->scoreValueLabel->setText(QString("%1 分").arg(value, 0, 'f', 1));
        ui->scoreStatusLabel->setText(isReasonable ? "分布合理" : "分布待优化");
        ui->scoreDetailLabel->setText("当前只读取到综合评分，未读取到详细指标。");
        ui->scorePanel->setVisible(true);
        return;
    }

    const QJsonObject result = document.object();
    const double value = result.value("totalScore").toDouble();
    const bool isReasonable = value >= 60.0;
    const QString color = isReasonable ? "#1f7a45" : "#b3261e";
    ui->scoreValueLabel->setStyleSheet(
        QString("color: %1; font-size: 24px; font-weight: bold;").arg(color));
    ui->scoreValueLabel->setText(QString("%1 分").arg(value, 0, 'f', 1));
    ui->scoreStatusLabel->setText(isReasonable ? "分布合理" : "分布待优化");
    ui->scoreDetailLabel->setText(QString(
        "医院数量：%1 家\n"
        "行政区面积：%2 km²\n"
        "服务半径：%3 km\n"
        "覆盖率：%4%\n"
        "平均最近距离：%5 km\n"
        "90%距离：%6 km\n"
        "最远距离：%7 km\n"
        "可达性：%8 分\n"
        "服务区均衡：%9 分\n"
        "盲区比例：%10%\n"
        "聚集度：%11 分")
        .arg(result.value("hospitalCount").toInt())
        .arg(result.value("areaKm2").toDouble(), 0, 'f', 1)
        .arg(result.value("serviceRadiusKm").toDouble(), 0, 'f', 1)
        .arg(result.value("coverageRate").toDouble() * 100, 0, 'f', 1)
        .arg(result.value("averageDistanceKm").toDouble(), 0, 'f', 1)
        .arg(result.value("p90DistanceKm").toDouble(), 0, 'f', 1)
        .arg(result.value("maxDistanceKm").toDouble(), 0, 'f', 1)
        .arg(result.value("accessibilityScore").toDouble(), 0, 'f', 1)
        .arg(result.value("voronoiBalanceScore").toDouble(), 0, 'f', 1)
        .arg(result.value("blindSpotRate").toDouble() * 100, 0, 'f', 1)
        .arg(result.value("clusteringScore").toDouble(), 0, 'f', 1));
    ui->scorePanel->setVisible(true);
}

void MainWindow::updateScorePanel()
{
    resetScorePanel();
    mapController->calculateRationalityResult([this](const QString& score) {
        if (!score.isEmpty()) {
            showScoreResult(score);
        }
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}
