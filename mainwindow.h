#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "mapcontroller.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    // 由 Qt Designer 生成的界面对象，负责访问 .ui 中的控件。
    Ui::MainWindow *ui;

    // 地图控制器封装 QWebEngineView 与网页 JavaScript 的交互逻辑。
    MapController *mapController;
};
#endif // MAINWINDOW_H
