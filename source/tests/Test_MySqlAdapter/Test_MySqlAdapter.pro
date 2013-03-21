#-------------------------------------------------
#
# Project created by QtCreator 2013-03-21T22:45:32
#
#-------------------------------------------------

QT       += sql testlib

QT       -= gui

TARGET = tst_test_mysqladapter
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

INCLUDEPATH += ../../ORM

SOURCES += tst_test_mysqladapter.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"
