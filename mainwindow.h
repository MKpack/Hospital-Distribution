#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
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
    // 隐藏并重置右侧合理性评分面板。
    void resetScorePanel();

    // 将网页端返回的评分结果显示到右侧评分面板。
    void showScoreResult(const QString &score);

    // 触发网页端评分计算，并在计算完成后刷新右侧评分面板。
    void updateScorePanel();

    // 由 Qt Designer 生成的界面对象，负责访问 .ui 中的控件。
    Ui::MainWindow *ui;

    // 地图控制器封装 QWebEngineView 与网页 JavaScript 的交互逻辑。
    MapController *mapController;

    // 当前已点击“获取行政区”的输入值，用于限制医院搜索必须基于已加载边界。
    QString loadedRegion;
};
#endif // MAINWINDOW_H
