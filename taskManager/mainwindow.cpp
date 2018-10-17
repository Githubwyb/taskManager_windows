#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->startDate->setDate(QDate::currentDate());
    ui->endDate->setDate(QDate::currentDate());
}

MainWindow::~MainWindow()
{
    delete ui;
}
