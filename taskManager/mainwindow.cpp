#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "log.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    LOG_INFO("Hello, mainWindow");
    ui->setupUi(this);
    ui->startDate->setDate(QDate::currentDate());
    ui->endDate->setDate(QDate::currentDate());
}

MainWindow::~MainWindow()
{
    LOG_INFO("~MainWindow");
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    LOG_INFO("clicked");
}
