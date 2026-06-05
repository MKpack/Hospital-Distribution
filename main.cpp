#include "mainwindow.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>

int main(int argc, char *argv[])
{
    // QApplication 负责初始化 Qt GUI 程序，并管理主事件循环。
    QApplication a(argc, argv);

    // 按系统语言尝试加载 Qt 翻译文件，方便后续扩展多语言界面。
    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "hospital_distribution_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }

    // 创建并显示主窗口，随后进入 Qt 事件循环。
    MainWindow w;
    w.show();
    return a.exec();
}
