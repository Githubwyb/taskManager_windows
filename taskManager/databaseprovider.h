#ifndef DATABASEPROVIDER_H
#define DATABASEPROVIDER_H

#include "baseinstance.h"

#include <QString>
#include <QSqlDatabase>

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

private:
    QString m_dbPath = "";
    std::shared_ptr<QSqlDatabase> m_pDB = nullptr;
};

#endif // DATABASEPROVIDER_H
