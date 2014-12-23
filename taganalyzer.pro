#-------------------------------------------------
#
# Project created by QtCreator 2014-12-01T22:11:52
#
#-------------------------------------------------

QT       += core gui xml xmlpatterns

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = taganalyzer
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    xmlreader.cpp \
    rawreader.cpp \
    abbottsoftwarereader.cpp \
    linechart.cpp

HEADERS  += mainwindow.h \
    xmlreader.h \
    rawreader.h \
    abbottsoftwarereader.h \
    linechart.h

FORMS    += mainwindow.ui
