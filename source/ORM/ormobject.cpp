#include "ormobject.h"

ORMObject::ORMObject(QObject *parent) :
    QObject(parent)
{
}

bool ORMObject::createTable()
{
    QHash<QString, QString> info; //QHash<name, type>
    for(int i = 1; i < metaObject()->propertyCount(); i++)
        info.insert(metaObject()->property(i).name(), metaObject()->property(i).typeName());
    return ORMDatabase::adapter->createTable(metaObject()->className(), info);
}
