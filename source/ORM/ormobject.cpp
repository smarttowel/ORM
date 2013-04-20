#include "ormobject.h"

#define RETURN_MANY \
    if(list.isEmpty()) \
        return false; \
    else \
    { \
        m_records = list; \
        translateRecToThisObj(list.first()); \
        return true; \
    } \

#define RETURN_ONE \
    if(list.isEmpty()) \
        return false; \
    else \
    { \
        m_records.clear(); \
        m_records.append(list.first()); \
        translateRecToThisObj(list.first()); \
        return true; \
    } \

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

int ORMObject::listSize()
{
    return m_records.size();
}

bool ORMObject::save()
{
    QHash<QString, QVariant> info;
    for(int i = 1; i < metaObject()->propertyCount(); i++)
        info.insert(QString(metaObject()->property(i).name()), metaObject()->property(i).read(this));
    if(id < 0)
    {
        id = ORMDatabase::adapter->addRecord(metaObject()->className(), info);
        return (id > 0);
    }
    else
        return ORMDatabase::adapter->updateRecord(metaObject()->className(), id, info);
}

bool ORMObject::update()
{
    if(id < 0)
        return false;
    QHash<QString, QVariant> info;
    for(int i = 1; i < metaObject()->propertyCount(); i++)
        info.insert(QString(metaObject()->property(i).name()), metaObject()->property(i).read(this));
    return ORMDatabase::adapter->updateRecord(metaObject()->className(), id, info);
}

bool ORMObject::find(int id)
{
    QList<QSqlRecord> list;
    list = ORMDatabase::adapter->find(metaObject()->className(), ORMWhere("id", ORMWhere::Equals, id).getWhereString());
    RETURN_ONE
}

bool ORMObject::findAll(ORMGroupBy group, ORMOrderBy order)
{
    QList<QSqlRecord> list;
    list = ORMDatabase::adapter->find(metaObject()->className(), group.getGroupString() + " " + order.getOrderString());
    RETURN_MANY
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

bool ORMObject::findBy(const QString fieldName, const QVariant value, ORMGroupBy group, ORMOrderBy order)
{
    QList<QSqlRecord> list = ORMDatabase::adapter->find(metaObject()->className(),
                                                        ORMWhere(fieldName, ORMWhere::Equals, value).getWhereString() + " " +
                                                        group.getGroupString() + " " + order.getOrderString());
    RETURN_MANY
}

bool ORMObject::findBy(const QString fieldName, const QVector<QVariant> &values, ORMGroupBy group, ORMOrderBy order)
{
    if(values.isEmpty())
        return false;
    QString whereString = "WHERE ";
    QVariant value;
    foreach(value, values)
        whereString += QString("%1 = '%2' OR ")
                .arg(fieldName)
                .arg(value.toString());
    whereString.resize(whereString.size() - 4);
    QList<QSqlRecord> list = ORMDatabase::adapter->find(metaObject()->className(), whereString + " " + group.getGroupString() + " " +
                                                        order.getOrderString());
    RETURN_MANY
}

bool ORMObject::findBy(const QHash<QString, QVariant> &params)
{
    if(params.isEmpty())
        return false;
    QString key;
    QString whereString = "WHERE ";
    QList<QSqlRecord> list;
    foreach(key, params.keys())
        whereString += QString("%1 = '%2' OR ")
                .arg(key)
                .arg(params.value(key).toString());
    whereString.resize(whereString.size() - 4);
    list = ORMDatabase::adapter->find(metaObject()->className(), whereString);
    RETURN_MANY
}

bool ORMObject::where(ORMWhere condition, ORMGroupBy group, ORMOrderBy order)
{
    QList<QSqlRecord> list;
    list = ORMDatabase::adapter->find(metaObject()->className(), condition.getWhereString() + " " + group.getGroupString() + " " +
                                      order.getOrderString());
    RETURN_MANY
}

bool ORMObject::exists()
{
    return !ORMDatabase::adapter->find(metaObject()->className(), "").isEmpty();
}

bool ORMObject::exists(int id)
{
    return !ORMDatabase::adapter->find(metaObject()->className(), ORMWhere("id", ORMWhere::Equals, id).getWhereString()).isEmpty();
}

bool ORMObject::exists(ORMWhere condition)
{
    return !ORMDatabase::adapter->find(metaObject()->className(), condition.getWhereString()).isEmpty();
}

bool ORMObject::updateProperty(QString fieldName, QVariant value)
{
    if(id < 0)
        return false;
    QHash<QString, QVariant> info;
    info.insert(fieldName, value);
    return ORMDatabase::adapter->updateRecord(metaObject()->className(), id, info);
}

bool ORMObject::remove()
{
    if(ORMDatabase::adapter->remove(metaObject()->className(), ORMWhere("id", ORMWhere::Equals, id).getWhereString()))
    {
        id = -1;
        return true;
    }
    else
        return false;
}

bool ORMObject::removeBy(ORMWhere condition)
{
    return ORMDatabase::adapter->remove(metaObject()->className(), condition.getWhereString());
}

bool ORMObject::removeAll() const
{
    return ORMDatabase::adapter->remove(metaObject()->className(), "");
}

int ORMObject::count() const
{
    return ORMDatabase::adapter->count(metaObject()->className(), "*");
}

int ORMObject::count(QString fieldName) const
{
    return ORMDatabase::adapter->count(metaObject()->className(), fieldName);
}

int ORMObject::count(ORMWhere condition) const
{
    return ORMDatabase::adapter->countBy(metaObject()->className(), condition.getWhereString());
}

double ORMObject::average(QString fieldName) const
{
    return ORMDatabase::adapter->calculation(ORMAbstractAdapter::Average, metaObject()->className(), fieldName, "");
}

double ORMObject::average(QString fieldName, ORMWhere condition) const
{
    return ORMDatabase::adapter->calculation(ORMAbstractAdapter::Average, metaObject()->className(),
                                             fieldName, condition.getWhereString());
}

double ORMObject::maximum(QString fieldName) const
{
    return ORMDatabase::adapter->calculation(ORMAbstractAdapter::Maximum, metaObject()->className(), fieldName, "");
}

double ORMObject::maximum(QString fieldName, ORMWhere condition) const
{
    return ORMDatabase::adapter->calculation(ORMAbstractAdapter::Maximum, metaObject()->className(),
                                             fieldName, condition.getWhereString());
}

double ORMObject::minimum(QString fieldName) const
{
    return ORMDatabase::adapter->calculation(ORMAbstractAdapter::Minimum, metaObject()->className(), fieldName, "");
}

double ORMObject::minimum(QString fieldName, ORMWhere condition) const
{
    return ORMDatabase::adapter->calculation(ORMAbstractAdapter::Minimum, metaObject()->className(),
                                             fieldName, condition.getWhereString());
}

double ORMObject::sum(QString fieldName) const
{
    return ORMDatabase::adapter->calculation(ORMAbstractAdapter::Sum, metaObject()->className(), fieldName, "");
}

double ORMObject::sum(QString fieldName, ORMWhere condition) const
{
    return ORMDatabase::adapter->calculation(ORMAbstractAdapter::Sum, metaObject()->className(),
                                             fieldName, condition.getWhereString());
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
