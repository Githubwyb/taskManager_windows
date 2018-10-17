/*
 * @Author WangYubo
 * @Date 09/17/2018
 * @Description log file
 */

#ifndef LOG_H
#define LOG_H

#include <QDebug>

#define LOG_INIT Log::log_init
#define LOG_DEBUG qDebug
#define LOG_WARN qWarning
#define LOG_INFO qInfo
#define LOG_ERROR qCritical
#define LOG_HEX(data, length) Log::log_hex(data, length)
#define LOG_BIN(data, length) Log::log_binary(data, length)

class Log {

public:
    /*
     * @function log初始化函数
     */
    static int log_init();

    /*
     * @function 以网络结构显示数据
     * @param data 数据首地址
     * @param length 数据长度
     */
    static void log_hex(const void *data, int length);

    /*
     * @function 以二进制显示数据
     * @param data 数据首地址
     * @param length 数据长度
     */
    static void log_binary(const void *data, int length);

    /*
     * @function 切分文件名，将前面的斜杠去除
     * @param fileName 文件名
     */
    static const char *splitFileName(const char *fileName);

private:
    /*
     * @function 自定义消息处理函数
     * @param type 消息重要程度
     * @param context 消息上下文
     * @param msg 消息主体
     */
    static void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg);
};

#endif //LOG_H
