#ifndef DATABASEPROVIDER_H
#define DATABASEPROVIDER_H

#include "baseinstance.h"

#include <QString>
#include <QSqlDatabase>
#include <QSqlQuery>

#include <memory>

class DatabaseProvider : public BaseInstance<DatabaseProvider>
{
public:
    ~DatabaseProvider();

    /*
     * @description 初始化数据库
     * @return 0，成功；其他，错误码
     */
    int initDB();

    /*
     * @description 数据库是否打开
     * @return true，打开；false，关闭
     */
    bool isOpen();

    /*
     * @description 数据库执行一条语句
     * @param sqlCmd 数据库执行语句
     * @return 0，成功；其他，错误码
     */
    int execute(const QString &sqlCmd);

    /*
     * @description 数据库查询一条语句
     * @param sqlCmd 数据库执行语句
     * @return nullptr,执行失败；结果指针
     */
    const std::shared_ptr<QSqlQuery> query(const QString &sqlCmd);

private:
    QString m_dbPath = "";
    std::shared_ptr<QSqlDatabase> m_pDB = nullptr;
};

#endif // DATABASEPROVIDER_H
