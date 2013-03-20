#-------------------------------------------------
#
# Project created by QtCreator 2013-03-20T18:41:13
#
#-------------------------------------------------

QT       += sql testlib

QT       -= gui

TARGET = tst_test_ormdatabase
CONFIG   += console
CONFIG   -= app_bundle

INCLUDEPATH += ../../ORM
TEMPLATE = app

SOURCES += tst_test_ormdatabase.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"
