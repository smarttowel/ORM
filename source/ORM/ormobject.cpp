#include "ormobject.h"

ORMObject::ORMObject(QObject *parent) :
    QObject(parent)
{
    id = -1;
}

bool ORMObject::createTable()
{
    QHash<QString, QString> info; //QHash<name, type>
    for(int i = 1; i < metaObject()->propertyCount(); i++)
        info.insert(metaObject()->property(i).name(), metaObject()->property(i).typeName());
    return ORMDatabase::adapter->createTable(metaObject()->className(), info);
}

int ORMObject::getId()
{
    return id;
}

bool ORMObject::save()
{
    QHash<QString, QVariant> info;
    for(int i = 1; i < metaObject()->propertyCount(); i++)
        info.insert(QString(metaObject()->property(i).name()), metaObject()->property(i).read(this));
    if(id == -1)
    {
        id = ORMDatabase::adapter->addRecord(metaObject()->className(), info);
        return (id > 0);
    }
    else
        return false;
}
