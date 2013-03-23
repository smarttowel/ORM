#-------------------------------------------------
#
# Project created by QtCreator 2013-03-22T23:23:58
#
#-------------------------------------------------

QT       += sql testlib

QT       -= gui

TARGET = tst_test_ormobject
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

INCLUDEPATH += ../../ORM

HEADERS += ../../ORM/ormobject.h

SOURCES += ../../ORM/ormobject.cpp tst_test_ormobject.cpp

DEFINES += SRCDIR=\\\"$$PWD/\\\"
