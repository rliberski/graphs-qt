#-------------------------------------------------
#
# Project created by QtCreator 2016-04-24T15:07:38
#
#-------------------------------------------------

QT       += core gui
QT       -= gui
CONFIG   += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = prgrafy
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    edge.cpp \
    graphicwindow.cpp \
    graphmanager.cpp \
    node.cpp

HEADERS  += mainwindow.h \
    edge.h \
    graphicwindow.h \
    graphmanager.h \
    node.h


