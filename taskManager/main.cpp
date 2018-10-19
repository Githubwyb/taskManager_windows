#include "mainwindow.h"
#include "init.h"
#include "log.h"
#include "accountmanager.h"
#include "sessionmanager.h"

#include <QApplication>
#include <QMessageBox>
#include <QDateTime>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    if (Init::getInstance().appInit() != 0) {
        QMessageBox::critical(nullptr, "出错", "程序初始化失败!");
        return -1;
    }

    LOG_INFO("Check account");
    if (AccountManager::getInstance().checkAvaliableAccount()) {
        if (SessionManager::getInstance().autoLogin() != 0) {
            LOG_ERROR("AutoLogin failed");
            return -1;
        }
    } else {
        std::shared_ptr<Account> pAccount = std::make_shared<Account>();
        QDateTime currentTime = QDateTime::currentDateTime();

        pAccount->setUsername("wangyubo");
        pAccount->setNickname("wyb");
        pAccount->setPassword("taskManager");
        pAccount->setLastLoginTime(currentTime.toTime_t());
        pAccount->setCreateTime(currentTime.toTime_t());

        std::shared_ptr<Session> pSession = std::make_shared<Session>(pAccount);
        if (pSession->login() != 0) {
            LOG_ERROR("Login failed");
            QMessageBox::critical(nullptr, "Error", "Login error");
            return -1;
        }
    }

    MainWindow w;
    w.show();

    return a.exec();
}

