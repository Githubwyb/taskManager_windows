#include "accountmanager.h"
#include "databaseprovider.h"
#include "SQLBuilder.hpp"
#include "constant.h"
#include "log.h"
#include "utils.h"

#include <QSqlRecord>

int AccountManager::createAccount(const std::shared_ptr<Account> &pAccount) {
    LOG_INFO("Begin createAccount");
    if (pAccount == nullptr) {
        LOG_ERROR("Account is null");
        return -1;
    }

    if (pAccount->getId().isEmpty()) {
        pAccount->setId(Utils::getRandString());
    }

    SQLBuilder sqlBuilder;
    sqlBuilder.clear();
    sqlBuilder.INSERT(F_DB_TABLE_ACCOUNT)
            .FIELDS({F_DB_ACCOUNT_ID,
                     F_DB_ACCOUNT_USERNAME,
                     F_DB_ACCOUNT_PASSWORD,
                     F_DB_ACCOUNT_NICKNAME,
                     F_DB_ACCOUNT_LASTLOGINTIME,
                     F_DB_ACCOUNT_CREATETIME,
                     F_DB_ACCOUNT_CONFIGS})
            .VALUES({pAccount->getId().toStdString(),
                     pAccount->getUsername().toStdString(),
                     pAccount->getPassword().toStdString(),
                     pAccount->getNickname().toStdString(),
                     std::to_string(pAccount->getLastLoginTime()),
                     std::to_string(pAccount->getCreateTime()),
                     pAccount->getConfigs().toStdString()});

    LOG_INFO("Exec %s", sqlBuilder.getSQL().toLatin1().data());
    int code = DatabaseProvider::getInstance().execute(sqlBuilder.getSQL());
    if (code != 0) {
        LOG_ERROR("Execute error, code %d", code);
        return -1;
    }

    LOG_INFO("CreateAccount success");
    return 0;
}

bool AccountManager::checkAvaliableAccount() {
    LOG_INFO("Begin checkAvaliableAccount");

    SQLBuilder sqlBuilder;
    sqlBuilder.clear();
    sqlBuilder.SELECT({F_DB_COUNT})
            .FROM({F_DB_TABLE_ACCOUNT});

    LOG_INFO("Query %s", sqlBuilder.getSQL().toLatin1().data());
    const std::shared_ptr<QSqlQuery> sqlQuery = DatabaseProvider::getInstance().query(sqlBuilder.getSQL());
    if (sqlQuery == nullptr) {
        LOG_ERROR("Execute error");
        return false;
    }

    if (!sqlQuery->first()) {
        LOG_ERROR("No result");
        return -1;
    }
    int count = sqlQuery->value(F_DB_COUNT).toInt();
    LOG_INFO("AvaliableAccount count %d", count);
    if (count == 0) {
        LOG_INFO("CheckAvaliableAccount false");
        return false;
    }

    LOG_INFO("CheckAvaliableAccount true");
    return true;
}
