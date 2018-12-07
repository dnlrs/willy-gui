#-------------------------------------------------
#
# Project created by QtCreator 2018-11-09T10:35:05
#
#-------------------------------------------------

QT       += core gui
QT       += charts
QT       += sql


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Application
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    position.cpp \
    device.cpp \
    chartview.cpp \
    dlctimer.cpp \
    istochart.cpp \
    confwdg.cpp

HEADERS += \
        mainwindow.h \
    position.h \
    device.h \
    chartview.h \
    dlctimer.h \
    istochart.h \
    confwdg.h

FORMS += \
        mainwindow.ui \
    mainwindow.ui \
    confwindow.ui
