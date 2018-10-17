#include "mainwindow.h"
#include "init.h"

#include <QApplication>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    if (Init::getInstance().appInit() != 0) {
        QMessageBox::critical(nullptr, "出错", "程序初始化失败!");
        return -1;
    }
    MainWindow w;
    w.show();

    return a.exec();
}

