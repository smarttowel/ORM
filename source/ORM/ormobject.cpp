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

bool ORMObject::dropTable()
{
    return ORMDatabase::adapter->dropTable(metaObject()->className());
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

bool ORMObject::find(int id)
{
    QList<QSqlRecord> list = ORMDatabase::adapter->find(metaObject()->className(), QString("id = %1").arg(id));
    if(list.empty())
        return false;
    else
    {
        translateRecToObj(list.first());
        return true;
    }
}

void ORMObject::translateRecToObj(QSqlRecord record)
{
    for(int i = 0; i < record.count(); i++)
        if(record.fieldName(i) != "id")
            setProperty(record.fieldName(i).toLocal8Bit().data(), record.value(i));
        else
            id = record.value(i).toInt();
}
