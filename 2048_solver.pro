#-------------------------------------------------
#
# Project created by QtCreator 2014-04-10T20:57:29
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = 2048_solver
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    gamedata.cpp \
    aidata.cpp \
    scorenode.cpp \
    node.cpp

HEADERS  += mainwindow.h \
        gamedata.h \
    aidata.h \
    scorenode.h \
    node.h

FORMS    += mainwindow.ui

INCLUDEPATH += C:/boost_1_55_0
