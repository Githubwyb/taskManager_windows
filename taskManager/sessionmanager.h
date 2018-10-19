#ifndef SESSIONMANAGER_H
#define SESSIONMANAGER_H

#include "session.h"
#include "baseinstance.h"

#include <vector>
#include <map>
#include <QMutex>

typedef std::map<QString, std::shared_ptr<Session>> SessionMap;

class SessionManager : public BaseInstance<SessionManager>
{

public:
    ~SessionManager();

    /*
     * @description 根据accountId获取session
     * @param accountId 账户id
     * @return 指向session的指针
     */
    const std::shared_ptr<Session> getSession(const QString &accountId);

    /*
     * @description 添加session到map中
     * @param pSession 指向session的指针
     * @return 0，成功；其他，错误码
     */
    int addSession(const std::shared_ptr<Session> &pSession);

    /*
     * @description 从map中删除一个session
     * @param accountId 账户id
     * @return 0，成功；其他，错误码
     */
    int deleteSession(const QString &accountId);

    /*
     * @description 自动登陆
     * @return 0，成功；其他，错误码
     */
    int autoLogin();

    /*
     * @description 登陆
     * @param 指向账户的指针
     * @return 0，成功；其他，错误码
     */
    int login(const std::shared_ptr<Account> &pAccount);

private:
    SessionMap m_sessionMap;    //储存当前session的map
    QMutex m_mutexSessionMap;   //对sessionMap加锁
};

#endif // SESSIONMANAGER_H
