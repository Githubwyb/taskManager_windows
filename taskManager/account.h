#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <QString>

class Account
{
public:
    Account();

    /*
     * @description 设置账户id
     * @param id 账户id
     */
    void setId(const QString &id);

    /*
     * @description 获取账户id
     * @return 账户id
     */
    const QString &getId() const;

    /*
     * @description 设置账户用户名
     * @param username 账户用户名
     */
    void setUsername(const QString &username);

    /*
     * @description 获取账户用户名
     * @return 账户用户名
     */
    const QString &getUsername() const;

    /*
     * @description 设置账户昵称
     * @param nickname 账户昵称
     */
    void setNickname(const QString &nickname);

    /*
     * @description 获取账户昵称
     * @return 账户昵称
     */
    const QString &getNickname() const;

    /*
     * @description 设置账户密码
     * @param password 账户密码
     */
    void setPassword(const QString &password);

    /*
     * @description 获取账户密码
     * @return 账户密码
     */
    const QString &getPassword() const;

    /*
     * @description 设置账户上次登录时间
     * @param date 账户上次登录时间
     */
    void setLastLoginTime(time_t date);

    /*
     * @description 获取账户上次登录时间
     * @return 账户上次登录时间
     */
    time_t getLastLoginTime() const;

    /*
     * @description 设置账户创建时间
     * @param date 账户创建时间
     */
    void setCreateTime(time_t date);

    /*
     * @description 获取账户创建时间
     * @return 账户创建时间
     */
    time_t getCreateTime() const;

    /*
     * @description 设置保留字段
     * @param configs 保留字段
     */
    void setConfigs(const QString &configs);

    /*
     * @description 获取保留字段
     * @return 保留字段
     */
    const QString &getConfigs() const;

private:
    QString m_id = "";              //账户id
    QString m_username = "";        //用户名
    QString m_nickname = "";        //昵称
    QString m_password = "";        //密码
    time_t m_lastLoginTime = 0;     //上次登录时间
    time_t m_createTime = 0;        //创建时间
    QString m_configs = "";           //保留字段

};

#endif // ACCOUNT_H
