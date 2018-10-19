#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "log.h"
#include "accountmanager.h"
#include "sessionmanager.h"

#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    LOG_INFO("Hello, mainWindow");
    QDateTime currentTime = QDateTime::currentDateTime();

    LOG_INFO("Init ui config");
    ui->setupUi(this);
    ui->startDate->setDateTime(currentTime);
    ui->endDate->setDateTime(currentTime);
}

MainWindow::~MainWindow()
{
    LOG_INFO("~MainWindow()");
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    LOG_INFO("clicked");
}
