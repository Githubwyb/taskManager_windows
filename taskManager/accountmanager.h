#ifndef ACCOUNTMANAGER_H
#define ACCOUNTMANAGER_H

#include "baseinstance.h"
#include "account.h"

#include <memory>
#include <vector>

class AccountManager : public BaseInstance<AccountManager>
{
public:
    ~AccountManager();

    /*
     * @description 创建一个账户
     * @param pAccount 指向account的指针
     * @return 0，成功；其他，错误码
     */
    int addAccount(const std::shared_ptr<Account> &pAccount);

    /*
     * @description 从数据库加载所有账户
     * @return 指向账户指针组成的列表
     */
    std::vector<std::shared_ptr<Account>> getAllAccount();

    /*
     * @description 检查是否有可用账户
     * @return true，有；false，没有
     */
    bool checkAvaliableAccount();
};

#endif // ACCOUNTMANAGER_H
