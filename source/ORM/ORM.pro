#-------------------------------------------------
#
# Project created by QtCreator 2013-03-15T17:28:06
#
#-------------------------------------------------

QT       += sql

QT       -= gui

TARGET = ORM
TEMPLATE = lib
CONFIG += staticlib

SOURCES += orm.cpp \
    ormobject.cpp \
    ormdatabase.cpp \
    ormabstractadapter.cpp

HEADERS += orm.h \
    ormobject.h \
    ormdatabase.h \
    ormabstractadapter.h
unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}
