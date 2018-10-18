#ifndef UTILS_H
#define UTILS_H

#include <QString>

#define SIZE_RAND_STRING 32

class Utils
{
public:
    Utils();

    /*
     * @description 获取一个随机字符串
     * @param size 字符串大小，默认为SIZE_RAND_STRING
     * @return 随机字符串
     */
    static const QString getRandString(int size = SIZE_RAND_STRING);
};

#endif // UTILS_H
