#-------------------------------------------------
#
# Project created by QtCreator 2016-03-05T23:55:13
#
#-------------------------------------------------

QT       += core gui\
         network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Network_Client
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

DISTFILES += \
    Client
