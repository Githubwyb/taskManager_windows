#include "sessionmanager.h"
#include "accountmanager.h"
#include "log.h"

SessionManager::~SessionManager() {
    LOG_INFO("~SessionManager()");
}

const std::shared_ptr<Session> SessionManager::getSession(const QString &accountId) {
    LOG_INFO("Begin getSession, accountId %s", accountId.toLatin1().data());
    if (accountId.isEmpty()) {
        LOG_ERROR("AccountId is empty");
        return nullptr;
    }

    m_mutexSessionMap.lock();
    if (m_sessionMap.count(accountId) == 0) {
        LOG_ERROR("No session with accountId %s", accountId.toLatin1().data());
        return nullptr;
    }

    std::shared_ptr<Session> pSession = m_sessionMap[accountId];
    m_mutexSessionMap.unlock();

    LOG_INFO("GetSession success");
    return pSession;
}

int SessionManager::addSession(const std::shared_ptr<Session> &pSession) {
    LOG_INFO("Begin addSession");
    const QString &accountId = pSession->getAccount()->getId();
    if (accountId.isEmpty()) {
        LOG_ERROR("AccountId is empty");
        return -1;
    }
    LOG_INFO("AccountId %s", accountId.toLatin1().data());

    m_mutexSessionMap.lock();
    if (m_sessionMap.count(accountId) != 0) {
        LOG_ERROR("Session is exist with accountId %s", accountId.toLatin1().data());
        return -1;
    }

    m_sessionMap[accountId] = pSession;
    m_mutexSessionMap.unlock();

    LOG_INFO("AddSession success");
    return 0;
}

int SessionManager::deleteSession(const QString &accountId) {
    LOG_INFO("Begin deleteSession, accountId %s", accountId.toLatin1().data());
    if (accountId.isEmpty()) {
        LOG_ERROR("AccountId is empty");
        return -1;
    }

    m_mutexSessionMap.lock();
    auto it = m_sessionMap.find(accountId);
    if (it == m_sessionMap.end()) {
        LOG_WARN("Session isn't exist with accountId %s", accountId.toLatin1().data());
        return 0;
    }

    m_sessionMap.erase(it);
    m_mutexSessionMap.unlock();

    LOG_INFO("DeleteSession success");
    return 0;
}

int SessionManager::autoLogin() {
    LOG_INFO("Begin autoLogin");
    const std::shared_ptr<Account> pAccount = AccountManager::getInstance().getAllAccount().front();

    std::shared_ptr<Session> pSession = std::make_shared<Session>(pAccount);
    if (pSession->login() != 0) {
        LOG_ERROR("Login failed");
        return -1;
    }

    if (addSession(pSession) != 0) {
        LOG_ERROR("AddSession failed");
        return -1;
    }

    LOG_INFO("AutoLogin success");
    return 0;
}

int SessionManager::login(const std::shared_ptr<Account> &pAccount) {
    LOG_INFO("Begin login");
    if (pAccount == nullptr) {
        LOG_ERROR("Account is null");
        return -1;
    }
    LOG_INFO("AccountId %s", pAccount->getId().toLatin1().data());

    std::shared_ptr<Session> pSession = std::make_shared<Session>(pAccount);
    if (pSession->login() != 0) {
        LOG_ERROR("Login failed");
        return -1;
    }

    if (AccountManager::getInstance().addAccount(pAccount) != 0) {
        LOG_ERROR("AddAccount failed");
        return -1;
    }

    if (addSession(pSession) != 0) {
        LOG_ERROR("AddSession failed");
        return -1;
    }

    LOG_INFO("Login success");
    return 0;
}
