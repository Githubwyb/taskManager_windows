#include "session.h"
#include "log.h"
#include "accountmanager.h"
#include "sessionmanager.h"

Session::Session()
{

}

const std::shared_ptr<Account> &Session::getAccount() const {
    return m_pAccount;
}

void Session::setAccount(const std::shared_ptr<Account> &pAccount) {
    m_pAccount = pAccount;
}

int Session::login() {
    LOG_INFO("Begin login");

    LOG_INFO("Login success");
    return 0;
}
