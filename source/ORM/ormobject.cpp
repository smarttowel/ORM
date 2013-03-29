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
        translateRecToThisObj(list.first());
        return true;
    }
}

bool ORMObject::first()
{
    QSqlRecord record = ORMDatabase::adapter->first(metaObject()->className());
    if(record.isNull("id"))
        return false;
    else
    {
        translateRecToThisObj(record);
        return true;
    }
}

bool ORMObject::last()
{
    QSqlRecord record = ORMDatabase::adapter->last(metaObject()->className());
    if(record.isNull("id"))
        return false;
    else
    {
        translateRecToThisObj(record);
        return true;
    }
}

bool ORMObject::findBy(QString fieldName, QVariant value)
{
    QSqlRecord record = ORMDatabase::adapter->findBy(metaObject()->className(), fieldName, value);
    if(record.isNull("id"))
        return false;
    else
    {
        translateRecToThisObj(record);
        return true;
    }
}

bool ORMObject::findBy(QHash<QString, QVariant> params)
{
    m_records.clear();
    QString key;
    QSqlRecord currentRecord;
    foreach(key, params.keys())
    {
        currentRecord = ORMDatabase::adapter->findBy(metaObject()->className(), key, params.value(key));
        if(!currentRecord.value("id").isNull())
            m_records.append(currentRecord);
    }
    if(m_records.isEmpty())
        return false;
    else
    {
        translateRecToThisObj(m_records.value(0));
        return true;
    }
}

void ORMObject::translateRecToThisObj(QSqlRecord record)
{
    for(int i = 0; i < record.count(); i++)
        if(record.fieldName(i) != "id")
            setProperty(record.fieldName(i).toLocal8Bit().data(), record.value(i));
        else
            id = record.value(i).toInt();
}

template<class T>
QList<T*> ORMObject::toList()
{
    QList<T*> result;
    for(int i = 0; i < m_records.size(); i++)
        result.append(translateRecToObj<T>(m_records.value(i)));
    return result;
}

template<class T>
T* ORMObject::translateRecToObj(QSqlRecord record)
{
    T *result = new T;
    for(int i = 0; i < record.count(); i++)
        if(record.fieldName(i) != "id")
            result->setProperty(record.fieldName(i).toLocal8Bit().data(), record.value(i));
        else
            result->id = record.value(i).toInt();
    return result;
}
