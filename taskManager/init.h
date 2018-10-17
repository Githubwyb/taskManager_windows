#ifndef INIT_H
#define INIT_H

#include <baseinstance.h>

class Init : public BaseInstance<Init>
{
public:

    /*
     * @description 整体程序初始化函数
     * @return 0，成功；其他，失败错误码
     */
    int appInit();
};

#endif // INIT_H
