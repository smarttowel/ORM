#include "ormobject.h"

ORMObject::ORMObject(QObject *parent) :
    QObject(parent)
{
}

bool ORMObject::createTable()
{
    QHash<QString, QString> fieldsInfo; //QHash<name, type>
    for(int i = 0; i < metaObject()->propertyCount(); i++)
        fieldsInfo.insert(metaObject()->property(i).name(), metaObject()->property(i).typeName());
    return ORMDatabase::adapter->createTable(fieldsInfo);
}
