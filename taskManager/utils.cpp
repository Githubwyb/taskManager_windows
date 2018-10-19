#include "utils.h"
#include "log.h"

#include <QDateTime>
#include <QString>

Utils::Utils()
{

}

const QString Utils::getRandString(int size) {
    const char charTable[] = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

    qsrand((unsigned) QDateTime::currentDateTime().toTime_t() + qrand());
    QString randString = "";
    for (int i = 0; i < size; ++i) {
        int index = qrand() % (sizeof(charTable) - 1);
        randString.append(QChar(charTable[index]));
    }

    return randString;
}

