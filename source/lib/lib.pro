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

HEADERS += orm.h \
    ormdatabase.h \
    ormabstractadapter.h \
    mysqladapter.h \
    ormwhere.h \
    ormgroupby.h \
    ormorderby.h \
    macros.h \
    ormobject.h

SOURCES += orm.cpp \
    ormdatabase.cpp \
    ormabstractadapter.cpp \
    mysqladapter.cpp \
    ormwhere.cpp \
    ormgroupby.cpp \
    ormorderby.cpp \
    ormobject.cpp

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}
