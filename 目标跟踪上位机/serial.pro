#-------------------------------------------------
#
# Project created by QtCreator 2016-10-22T10:49:00
#
#-------------------------------------------------

QT       += core gui
QT       += serialport
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = serial
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    drawwidget.cpp

HEADERS  += mainwindow.h \
    drawwidget.h

FORMS    += mainwindow.ui
