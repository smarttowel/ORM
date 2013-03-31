#include "ormobject.h"

ORMObject::ORMObject(QObject *parent) :
    QObject(parent)
{
    id = -1;
}

bool ORMObject::createTable() const
{
    QHash<QString, QString> info; //QHash<name, type>
    for(int i = 1; i < metaObject()->propertyCount(); i++)
        info.insert(metaObject()->property(i).name(), metaObject()->property(i).typeName());
    return ORMDatabase::adapter->createTable(metaObject()->className(), info);
}

bool ORMObject::dropTable() const
{
    return ORMDatabase::adapter->dropTable(metaObject()->className());
}

int ORMObject::getId() const
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
    QList<QSqlRecord> list;
    list = ORMDatabase::adapter->find(metaObject()->className(), QString("id = %1").arg(id));
    if(list.isEmpty())
        return false;
    else
    {
        m_records.clear();
        m_records.append(list.first());
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
        m_records.clear();
        m_records.append(record);
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
        m_records.clear();
        m_records.append(record);
        translateRecToThisObj(record);
        return true;
    }
}

bool ORMObject::findBy(const QString fieldName, const QVariant value)
{
    QList<QSqlRecord> list = ORMDatabase::adapter->find(metaObject()->className(), QString("%1 = '%2'")
                                                   .arg(fieldName)
                                                   .arg(value.toString()));
    if(list.isEmpty())
        return false;
    else
    {
        m_records = list;
        translateRecToThisObj(list.first());
        return true;
    }
}

bool ORMObject::findBy(const QHash<QString, QVariant> &params)
{
    QString key;
    QString whereString;
    QList<QSqlRecord> list;
    foreach(key, params.keys())
        whereString += QString("%1 = '%2' OR ")
                .arg(key)
                .arg(params.value(key).toString());
    whereString.resize(whereString.size() - 4);
    list = ORMDatabase::adapter->find(metaObject()->className(), whereString);
    if(list.isEmpty())
        return false;
    else
    {
        m_records = list;
        translateRecToThisObj(m_records.first());
        return true;
    }
}

bool ORMObject::where(ORMWhere condition)
{
    QList<QSqlRecord> list;
    list = ORMDatabase::adapter->find(metaObject()->className(), condition.getWhereCondition());
    if(list.isEmpty())
        return false;
    else
    {
        m_records = list;
        translateRecToThisObj(m_records.first());
        return true;
    }
}

void ORMObject::translateRecToThisObj(const QSqlRecord &record)
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
T* ORMObject::translateRecToObj(const QSqlRecord &record)
{
    T *result = new T;
    for(int i = 0; i < record.count(); i++)
        if(record.fieldName(i) != "id")
            result->setProperty(record.fieldName(i).toLocal8Bit().data(), record.value(i));
        else
            result->id = record.value(i).toInt();
    return result;
}
