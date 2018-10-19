#ifndef SESSION_H
#define SESSION_H

#include "account.h"

#include <memory>

class Session
{
public:
    Session();

    Session(const std::shared_ptr<Account> &pAccount) : m_pAccount(pAccount) {}

    /*
     * @description 获取账户
     * @return 指向账户的指针
     */
    const std::shared_ptr<Account> &getAccount() const;

    /*
     * @description 设置账户
     * @param pAccount 指向账户的指针
     */
    void setAccount(const std::shared_ptr<Account> &pAccount);

    /*
     * @description 登陆
     * @return 0，成功；其他，错误码
     */
    int login();

private:
    std::shared_ptr<Account> m_pAccount = nullptr;  //储存账户的指针
};

#endif // SESSION_H
