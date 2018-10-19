#-------------------------------------------------
#
# Project created by QtCreator 2018-10-15T12:46:32
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = taskManager
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS QT_MESSAGELOGCONTEXT

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    init.cpp \
    databaseprovider.cpp \
    log.cpp \
    SQLBuilder.cpp \
    accountmanager.cpp \
    account.cpp \
    utils.cpp \
    session.cpp \
    sessionmanager.cpp

HEADERS += \
        mainwindow.h \
    init.h \
    databaseprovider.h \
    baseinstance.h \
    log.h \
    version.h \
    constant.h \
    SQLBuilder.hpp \
    accountmanager.h \
    account.h \
    utils.h \
    session.h \
    sessionmanager.h

FORMS += \
        mainwindow.ui
