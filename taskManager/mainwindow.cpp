#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "log.h"
#include "accountmanager.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    LOG_INFO("Hello, mainWindow");
    QDateTime currentTime = QDateTime::currentDateTime();

    LOG_INFO("Check account");
    if (AccountManager::getInstance().checkAvaliableAccount()) {

    } else {
        std::shared_ptr<Account> pAccount = std::make_shared<Account>();
        pAccount->setUsername("wangyubo");
        pAccount->setNickname("wyb");
        pAccount->setPassword("taskManager");
        pAccount->setLastLoginTime(currentTime.toTime_t());
        pAccount->setCreateTime(currentTime.toTime_t());
        AccountManager::getInstance().createAccount(pAccount);
    }

    LOG_INFO("Init ui config");
    ui->setupUi(this);
    ui->startDate->setDateTime(currentTime);
    ui->endDate->setDateTime(currentTime);


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
