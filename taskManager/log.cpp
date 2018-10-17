/*
 * @Author WangYubo
 * @Date 10/17/2018
 * @Description log相关程序
 */

#include "log.h"
#include "version.h"
#include "constant.h"

#include <QString>
#include <QMutex>
#include <QFile>
#include <QDate>
#include <QDir>
#include <QTextStream>

// 自定义消息处理程序
void Log::myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    static QMutex mutex;
    mutex.lock();

    QString text = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");

    switch (type)
    {
    case QtDebugMsg:
        text += QString("[D]");
        break;

    case QtInfoMsg:
        text += QString("[I]");
        break;

    case QtWarningMsg:
        text += QString("[W]");
        break;

    case QtCriticalMsg:
        text += QString("[E]");
        break;

    case QtFatalMsg:
        text += QString("[F]");
    }

    text += QString("[%1:%2 (%3)] %4").arg(splitFileName(context.file)).arg(context.line).arg(context.function).arg(msg);

    //目录为程序目录的log下
    QString logFile = QString("%1%2_").arg(F_PATH_LOG).arg(F_PROGRAM_NAME) + QDateTime::currentDateTime().toString("yyyy_MM_dd") + ".log";
    QFile file(logFile);
    file.open(QIODevice::WriteOnly | QIODevice::Append);
    //    if (file.size() >= 10 * 1024 * 1024) //文件达到10M后先备份再清空
    //    {
    //        QFile::remove("log_00.txt");//删除原来的备份文件
    //        file.copy("log_00.txt");    //拷贝文件至备份文件
    //        file.resize(0);             //清空文件内容
    //    }
    QTextStream text_stream(&file);
    text_stream << text << "\r\n";
    file.flush();
    file.close();

    mutex.unlock();
}

int Log::log_init() {
    if(!QDir().mkpath(F_PATH_LOG)) {
        return -1;
    }

    //目录为程序目录的log下
    QString logFile = QString("%1%2_").arg(F_PATH_LOG).arg(F_PROGRAM_NAME) + QDateTime::currentDateTime().toString("yyyy_MM_dd") + ".log";
    QFile file(logFile);

    if(!file.open(QIODevice::WriteOnly | QIODevice::Append)) {
        return -1;
    }

    //    if (file.size() >= 10 * 1024 * 1024) //文件达到10M后先备份再清空
    //    {
    //        QFile::remove("log_00.txt");//删除原来的备份文件
    //        file.copy("log_00.txt");    //拷贝文件至备份文件
    //        file.resize(0);             //清空文件内容
    //    }
    QTextStream text_stream(&file);
    text_stream << "\r\n\r\n======================================\r\n"
                << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")
                << QString(" version: %1").arg(VERSION_STR)
                << "\r\n======================================\r\n";

    if(!file.flush()) {
        return -1;
    }
    file.close();

    //注册message处理函数
    qInstallMessageHandler(myMessageOutput);
    return 0;
}

/*
 * The hex log is in the following format:
 *
 *     0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F      0123456789ABCDEF
 * 01  aa 55 01 00 00 00 25 00 38 36 35 30 36 37 30 32     .U....%.86506702
 * 02  30 34 39 30 31 36 38 30 00 00 00 00 00 00 00 00     04901680........
 * 03  00 00 00 00 00 00 00 00 00 00 00 00                 ............
 *
 */
void Log::log_hex(const void *data, int length) {
    const char *pData = (const char *) data;

    QString outputStr = QString("\r\n    ");
    for (int i = 0; i < 16; i++) {
        outputStr += QString().sprintf("%X  ", i);
    }

    outputStr += QString().sprintf("    ");
    for (int i = 0; i < 16; i++) {
        outputStr += QString().sprintf("%X", i);
    }

    outputStr += QString().sprintf("\r\n");

    for (int i = 0; i < length; i += 16) {
        outputStr += QString().sprintf("%02d  ", i / 16 + 1);
        int j = 0;
        for (j = i; j < i + 16 && j < length; j++) {
            outputStr += QString().sprintf("%02x ", pData[j] & 0xff);
        }
        if (j == length && length % 16) {
            for (j = 0; j < (16 - length % 16); j++) {
                outputStr += QString().sprintf("   ");
            }
        }
        outputStr += QString().sprintf("    ");
        for (j = i; j < i + 16 && j < length; j++) {
            if (pData[j] < 32 || pData[j] >= 127) {
                outputStr += QString().sprintf(".");
            } else {
                outputStr += QString().sprintf("%c", pData[j] & 0xff);
            }
        }

        outputStr += QString().sprintf("\r\n");
    }

    qDebug("%s", outputStr.toLatin1().data());
}

/*
 * 打印二进制数据接口
 * 当前仅支持打印前32个字节的二进制数据
 */
#define MAX_PRINTED_BIN_LENGTH (32)

void Log::log_binary(const void *data, int length) {
    const char *point = (const char *)data;

    QString outputStr = "\r\n";
    int i = 0;
    for (i = 0; i < length && i < MAX_PRINTED_BIN_LENGTH; i++)
    {
        outputStr += QString().sprintf("%02x ", point[i]);
    }

    if (i >= MAX_PRINTED_BIN_LENGTH)
    {
        outputStr += QString().sprintf("......");
    }

    qDebug("%s", outputStr.toLatin1().data());
}

const char *Log::splitFileName(const char *fileName) {
    const char *pChar = fileName;
    pChar = (strrchr(pChar, '/') ? strrchr(pChar, '/') + 1 : (strrchr(pChar, '\\') ? strrchr(pChar, '\\') + 1 : pChar));
    return pChar;
}
