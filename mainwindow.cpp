#include "mainwindow.h"
#include "./ui_mainwindow.h"

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
}

MainWindow::~MainWindow()
{
    delete ui;
}
