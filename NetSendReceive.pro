#-------------------------------------------------
#
# Project created by QtCreator 2015-11-14T10:37:52
#
#-------------------------------------------------

QT += core gui network widgets

TARGET = NetSendReceive
TEMPLATE = app

VERSION = 1.0
QMAKE_TARGET_COPYRIGHT = copyright 2017 LAAS/CNRS

DEFINES += "NSR_VERSION=\\\"1.0\\\""

SOURCES +=\
    Common/main.cpp \
    Common/mainwindow.cpp \
    Common/tablemodel.cpp \
    Common/datatableview.cpp \
    Common/datatablewidget.cpp \
    Common/datatablewithlist.cpp \
    Tcp/tcpwidget.cpp \
    Udp/udpreceivewidget.cpp \
    Udp/udpsendwidget.cpp \
    Udp/udpwidget.cpp \
    Tcp/tcpclientwidget.cpp \
    Tcp/tcpserverwidget.cpp \
    Tcp/tcpsendreceivewidget.cpp \
    Common/logwidget.cpp

HEADERS += \
    Common/mainwindow.h \
    Common/tablemodel.h \
    Common/datatableview.h \
    Common/datatablewidget.h \
    Common/datatablewithlist.h \
    Common/networkconfig.h \
    Tcp/tcpwidget.h \
    Udp/udpreceivewidget.h \
    Udp/udpsendwidget.h \
    Udp/udpwidget.h \
    Tcp/tcpclientwidget.h \
    Tcp/tcpserverwidget.h \
    Tcp/tcpsendreceivewidget.h \
    Common/logwidget.h

INCLUDEPATH += \
    Common/ \
    Tcp/ \
    Udp/

CONFIG += c++11

DISTFILES += \
    LICENSE.txt

RESOURCES += \
    languages.qrc

TRANSLATIONS += \
    Languages/français.ts
