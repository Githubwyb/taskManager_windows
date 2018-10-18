#include "init.h"
#include "log.h"
#include "databaseprovider.h"
#include "utils.h"

int Init::appInit() {
    if(LOG_INIT() != 0) {
        return -1;
    }
    LOG_INFO("Log init success");

    if(DatabaseProvider::getInstance().initDB() != 0) {
        LOG_ERROR("Database init error");
        return -1;
    }
    LOG_INFO("Database init success");

    return 0;
}
