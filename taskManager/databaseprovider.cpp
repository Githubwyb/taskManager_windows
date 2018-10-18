#include "databaseprovider.h"
#include "constant.h"
#include "log.h"

#include <QDir>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

DatabaseProvider::~DatabaseProvider()
{
    LOG_INFO("~DatabaseProvider()");
    if (isOpen()) {
        m_pDB->close();
    }
}

int DatabaseProvider::initDB() {
    LOG_INFO("Begin initDB");
    if(!QDir().mkpath(F_PATH_DB)) {
        LOG_DEBUG("Database path create error");
        return -1;
    }

    //创建一个SQLite数据库连接
    m_pDB = std::make_shared<QSqlDatabase>(QSqlDatabase::addDatabase("QSQLITE"));

    //数据库连接命名
    m_dbPath = QString("%1%2").arg(F_PATH_DB).arg(F_DB_NAME);
    m_pDB->setDatabaseName(m_dbPath);

    if (!m_pDB->open()) {
        LOG_ERROR("Database open failed, path %s", m_dbPath.toLatin1().data());
        return -1;
    }
    LOG_INFO("Database open success, path %s", m_dbPath.toLatin1().data());

    QSqlQuery sqlQuery(*m_pDB);
    bool result = sqlQuery.exec("CREATE TABLE IF NOT EXISTS TM_Account ("
                                "id TEXT PRIMARY KEY UNIQUE, "
                                "username TEXT, "
                                "password TEXT, "
                                "nickname TEXT, "
                                "lastLoginTime INTEGER, "
                                "createTime INTEGER, "
                                "configs TEXT)");
    if (!result) {
        LOG_ERROR("Create table TM_Account failed, code %d, %s", sqlQuery.lastError().type(), sqlQuery.lastError().text().toLatin1().data());
        return -1;
    }
    LOG_INFO("Create table TM_Account success");

    result = sqlQuery.exec("CREATE TABLE IF NOT EXISTS TM_task ("
                           "id TEXT PRIMARY KEY UNIQUE, "
                           "accountId TEXT NOT NULL, "
                           "subject TEXT, "
                           "abstract TEXT, "
                           "type INTEGER, "
                           "createTime INTEGER, "
                           "modifyTime INTEGER, "
                           "startTime INTEGER, "
                           "endTime INTEGER, "
                           "importance INTEGER, "
                           "isStared INTEGER, "
                           "isDeleted INTEGER, "
                           "configs TEXT)");
    if (!result) {
        LOG_ERROR("Create table TM_task failed, code %d, %s", sqlQuery.lastError().type(), sqlQuery.lastError().text().toLatin1().data());
        return -1;
    }
    LOG_INFO("Create table TM_task success");

    return 0;
}

bool DatabaseProvider::isOpen() {
    if (m_pDB != nullptr && m_pDB->isOpen()) {
        return true;
    }

    return false;
}

int DatabaseProvider::execute(const QString &sqlCmd) {
    if (!isOpen()) {
        LOG_ERROR("Database isn't open");
        return -1;
    }

    QSqlQuery sqlQuery(*m_pDB);
    bool result = sqlQuery.exec(sqlCmd);
    if (!result) {
        LOG_ERROR("Exec failed, code %d, %s", sqlQuery.lastError().type(), sqlQuery.lastError().text().toLatin1().data());
        return -1;
    }

    return 0;
}

const std::shared_ptr<QSqlQuery> DatabaseProvider::query(const QString &sqlCmd) {
    if (!isOpen()) {
        LOG_ERROR("Database isn't open");
        return nullptr;
    }

    QSqlQuery sqlQuery(*m_pDB);
    bool result = sqlQuery.exec(sqlCmd);
    if (!result) {
        LOG_ERROR("Query failed, code %d, %s", sqlQuery.lastError().type(), sqlQuery.lastError().text().toLatin1().data());
        return nullptr;
    }

    return std::make_shared<QSqlQuery>(sqlQuery);
}
