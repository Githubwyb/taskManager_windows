#include "account.h"

Account::Account()
{

}

void Account::setId(const QString &id) {
    m_id = id;
}

const QString &Account::getId() const {
    return m_id;
}

void Account::setUsername(const QString &username) {
    m_username = username;
}

const QString &Account::getUsername() const {
    return m_username;
}

void Account::setNickname(const QString &nickname) {
    m_nickname = nickname;
}

const QString &Account::getNickname() const {
    return m_nickname;
}

void Account::setPassword(const QString &password) {
    m_password = password;
}

const QString &Account::getPassword() const {
    return m_password;
}

void Account::setLastLoginTime(time_t date) {
    m_lastLoginTime = date;
}

time_t Account::getLastLoginTime() const {
    return m_lastLoginTime;
}

void Account::setCreateTime(time_t date) {
    m_createTime = date;
}

time_t Account::getCreateTime() const {
    return m_createTime;
}

void Account::setConfigs(const QString &configs) {
    m_configs = configs;
}

const QString &Account::getConfigs() const {
    return m_configs;
}
