#-------------------------------------------------
#
# Project created by QtCreator 2014-10-30T01:45:32
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Hangman3
TEMPLATE = app


SOURCES += main.cpp\
        dialog.cpp
CONFIG += console;
HEADERS  += \
        dialog.h

FORMS    += dialog.ui

RESOURCES += \
    Words.qrc \
    Pictures.qrc \
    fonts.qrc
