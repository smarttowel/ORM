#ifndef ORMOBJECT_H
#define ORMOBJECT_H

#include <QObject>
#include <QMetaProperty>
#include <QSqlRecord>
#include <QVector>
#include "ormdatabase.h"
#include "ormwhere.h"
#include "ormgroupby.h"
#include "ormorderby.h"
#include "macros.h"

/*!
   \class ORMObject
   \brief The ORMObject is a base class for all your models.

   For create model, you must inherits by this class.
   Example:
   \code
   Car : public ORMObject
   {
    Car();
    ORM_PROPERTY(QString, brand)
    ORM_PROPERTY(QDate, year)
    ORM_PROPERTY(QString, color)
   }
   \endcode
 */

template<class ModelName>
class ORMObject : public QObject
{
public:
    /*!
       Creates empty ORMObject with \a parent QObject.
     */
    explicit ORMObject(QObject *parent = 0) : QObject(parent) {id = -1; }
    /*!
       Creates table associated with model.

       Returns true if table created, otherwise return false.
     */
    bool createTable() const
    {
        QHash<QString, QString> info; //QHash<name, type>
        for(int i = 1; i < metaObject()->propertyCount(); i++)
            info.insert(metaObject()->property(i).name(), metaObject()->property(i).typeName());
        if(!ORMDatabase::adapter->createTable(metaObject()->className(), info))
            return false;
        QString tableNamePattern;
        QString tempString;
        for(int i = 0; i < metaObject()->classInfoCount(); i++)
        {
            tempString = metaObject()->classInfo(i).name();
            if(tempString.contains("HAS_ONE:") || tempString.contains("HAS_MANY:"))
                tableNamePattern = "%1_" + QString(metaObject()->classInfo(i).name()).remove(':') + "_%2";
            else
                continue;
            if(!ORMDatabase::adapter->createTableRelations(tableNamePattern
                                                           .arg(metaObject()->className())
                                                           .arg(metaObject()->classInfo(i).value())))
            return false;
        }
        return true;
    }
    /*!
       Deletes table associated with model.

       Returns true if table deleted, otherwise return false.
     */
    bool dropTable() const
    {
        if(!ORMDatabase::adapter->dropTable(metaObject()->className()))
            return false;
        QString tableNamePattern;
        QString tempString;
        for(int i = 0; i < metaObject()->classInfoCount(); i++)
        {
            tempString = metaObject()->classInfo(i).name();
            if(tempString.contains("HAS_ONE:") || tempString.contains("HAS_MANY:"))
                tableNamePattern = "%1_" + QString(metaObject()->classInfo(i).name()).remove(':') + "_%2";
            else
                continue;
            if(!ORMDatabase::adapter->dropTable(tableNamePattern
                                                .arg(metaObject()->className())
                                                .arg(metaObject()->classInfo(i).value())))
                return false;
        }
        return true;
    }
    /*!
       Returns object id.
     */
    void setId(qlonglong newId)
    {
        id = newId;
    }
    int getId() const
    {
        return id;
    }
    /*!
       Creates new record in table. Read all meta-property from model and save their into table.

       Returns true if success, otherwise return false.
     */
    bool save()
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
    /*!
       Updates existing record in table with object's \a id.

       Returns true if success, otherwise return false.
     */
    bool update()
    {
        if(id < 0)
            return false;
        QHash<QString, QVariant> info;
        for(int i = 1; i < metaObject()->propertyCount(); i++)
            info.insert(QString(metaObject()->property(i).name()), metaObject()->property(i).read(this));
        return ORMDatabase::adapter->updateRecord(metaObject()->className(), id, info);
    }
    /*!
       Finds object by \a id.

       Returns true if object is found, otherwise return false.
     */
    ModelName* find(int id)
    {
        QList<QSqlRecord> list;
        list = ORMDatabase::adapter->find(metaObject()->className(), ORMWhere("id", ORMWhere::Equals, id).getWhereString());
        if(list.isEmpty())
            return new ModelName;
        else
            return translateRecToObj<ModelName>(list.first());
    }
    /*!
       Finds all records in table. List of objects you can get by calling toList().

       Returns true is success, otherwise retur false. If table is empty return false.
     */
    QList<ModelName*> findAll(ORMGroupBy group = ORMGroupBy(), ORMOrderBy order = ORMOrderBy())
    {
        QList<QSqlRecord> list = ORMDatabase::adapter->find(metaObject()->className(), group.getGroupString() + " " + order.getOrderString());
        QList<ModelName*> returnList;
        for(int i = 0; i < list.size(); i++)
            returnList.append(translateRecToObj<ModelName>(list.value(i)));
        return returnList;
    }
    /*!
       Finds first object in table.

       Returns true if object is found, otherwise return false.
     */
    ModelName* first()
    {
        QSqlRecord record = ORMDatabase::adapter->first(metaObject()->className());
        if(record.isNull("id"))
            return new ModelName;
        else
            return translateRecToObj<ModelName>(record);
    }
    /*!
       Finds last object in table.

       Returns true if object is found, otherwise return false.
     */
    ModelName* last()
    {
        QSqlRecord record = ORMDatabase::adapter->last(metaObject()->className());
        if(record.isNull("id"))
            return new ModelName;
        else
            return translateRecToObj<ModelName>(record);
    }
    /*!
       Finds object by some field and value. If there is more than one object, you can get them by toList().

       Returns true if object is found, otherwise return false.
     */
    QList<ModelName*> findBy(const QString fieldName, const QVariant value, ORMGroupBy group = ORMGroupBy(), ORMOrderBy order = ORMOrderBy())
    {
        QList<QSqlRecord> list = ORMDatabase::adapter->find(metaObject()->className(),
                                                            ORMWhere(fieldName, ORMWhere::Equals, value).getWhereString() + " " +
                                                            group.getGroupString() + " " + order.getOrderString());
        QList<ModelName*> returnList;
        for(int i = 0; i < list.size(); i++)
            returnList.append(translateRecToObj<ModelName>(list.value(i)));
        return returnList;
    }
    /*!
       Finds objects by vector of values. If found more than one object, you can get them by toList().

       Returns true if object is found, otherwise return false.
     */
    QList<ModelName*> findBy(const QString fieldName, const QVector<QVariant> &values, ORMGroupBy group = ORMGroupBy(), ORMOrderBy order = ORMOrderBy())
    {
        QList<ModelName*> returnList;
        if(values.isEmpty())
            return returnList;
        QString whereString = "WHERE ";
        QVariant value;
        foreach(value, values)
            whereString += QString("%1 = '%2' OR ")
                    .arg(fieldName)
                    .arg(value.toString());
        whereString.resize(whereString.size() - 4);
        QList<QSqlRecord> list = ORMDatabase::adapter->find(metaObject()->className(), whereString + " " + group.getGroupString() + " " +
                                                            order.getOrderString());
        for(int i = 0; i < list.size(); i++)
            returnList.append(translateRecToObj<ModelName>(list.value(i)));
        return returnList;
    }
    /*!
       Finds object by many fields and values. If there is more than one object, you can get them by toList().
       \a params - QHash<fieldName, value>, of which will be searched.

       Returns true if object is found, otherwise return false.
     */
    QList<ModelName*> findBy(const QHash<QString, QVariant> &params)
    {
        QList<ModelName*> returnList;
        if(params.isEmpty())
            return returnList;
        QString key;
        QString whereString = "WHERE ";
        foreach(key, params.keys())
            whereString += QString("%1 = '%2' OR ")
                    .arg(key)
                    .arg(params.value(key).toString());
        whereString.resize(whereString.size() - 4);
        QList<QSqlRecord> list = ORMDatabase::adapter->find(metaObject()->className(), whereString);
        for(int i = 0; i < list.size(); i++)
            returnList.append(translateRecToObj<ModelName>(list.value(i)));
        return returnList;
    }
    /*!
       Finds object with use WHERE expression. If there is more than one object, you can get them by toList().
       For ORMWhere defined && and || operators, so you use next code:
       \code
       Model.where(ORMWhere(...) || ORMWhere(...));
       \endcode
       or
       \code
       Model.where(ORMWhere(...) && ORMWhere(...));
       \endcode

       Returns true if object is found, otherwise return false.
     */
    QList<ModelName*> where(ORMWhere condition, ORMGroupBy group = ORMGroupBy(), ORMOrderBy order = ORMOrderBy())
    {
        QList<QSqlRecord> list = ORMDatabase::adapter->find(metaObject()->className(), condition.getWhereString() + " " + group.getGroupString() + " " +
                                          order.getOrderString());
        QList<ModelName*> returnList;
        for(int i = 0; i < list.size(); i++)
            returnList.append(translateRecToObj<ModelName>(list.value(i)));
        return returnList;
    }
    /*!
       Returns true if table is empty, otherwise return false.
     */
    bool exists()
    {
        return !ORMDatabase::adapter->find(metaObject()->className(), "").isEmpty();
    }
    /*!
       Returns true if object with given \a id exist, otherwise return false.
     */
    bool exists(int id)
    {
        return !ORMDatabase::adapter->find(metaObject()->className(), ORMWhere("id", ORMWhere::Equals, id).getWhereString()).isEmpty();
    }
    /*!
       Verification existence of objects with specified conditions.

       Returns true if exist at least one object, otherwise return false.
     */
    bool exists(ORMWhere condition)
    {
        return !ORMDatabase::adapter->find(metaObject()->className(), condition.getWhereString()).isEmpty();
    }
    /*!
       Immediately updates object field in table.

       Returns true if property updated, otherwise return false.
     */
    bool updateProperty(QString fieldName, QVariant value)
    {
        if(id < 0)
            return false;
        QHash<QString, QVariant> info;
        info.insert(fieldName, value);
        return ORMDatabase::adapter->updateRecord(metaObject()->className(), id, info);
    }
    /*!
       Removes table's record with object's id.

       Returns true if success, otherwise return false.
     */
    bool remove()
    {
        if(ORMDatabase::adapter->remove(metaObject()->className(), ORMWhere("id", ORMWhere::Equals, id).getWhereString()))
        {
            id = -1;
            return true;
        }
        else
            return false;
    }
    /*!
       Removes all objects, that match \a condition.

       Returns true if success, otherwise return false.
     */
    bool removeBy(ORMWhere condition)
    {
        return ORMDatabase::adapter->remove(metaObject()->className(), condition.getWhereString());
    }
    /*!
       Removes all records from table.

       Returns true is success, otherwise return false.
     */
    bool removeAll() const
    {
        return ORMDatabase::adapter->remove(metaObject()->className(), "");
    }
    /*!
       Returns number of objects in table.
     */
    int count() const
    {
        return ORMDatabase::adapter->count(metaObject()->className(), "*");
    }
    /*!
       Returns number of not null fields with given \a fieldName column.
     */
    int count(QString fieldName) const
    {
        return ORMDatabase::adapter->count(metaObject()->className(), fieldName);
    }
    /*!
       Returns number of objects that match \a condition.
     */
    int count(ORMWhere condition) const
    {
        return ORMDatabase::adapter->countBy(metaObject()->className(), condition.getWhereString());
    }
    /*!
       Returns average value from given \a fieldName column.
     */
    double average(QString fieldName) const
    {
        return ORMDatabase::adapter->calculation(ORMAbstractAdapter::Average, metaObject()->className(), fieldName, "");
    }
    /*!
       Returns average value from given \a fieldName column that match \a condition.
     */
    double average(QString fieldName, ORMWhere condition) const
    {
        return ORMDatabase::adapter->calculation(ORMAbstractAdapter::Average, metaObject()->className(),
                                                 fieldName, condition.getWhereString());
    }
    /*!
       Returns maximum value from given \a fieldName column.
     */
    double maximum(QString fieldName) const
    {
        return ORMDatabase::adapter->calculation(ORMAbstractAdapter::Maximum, metaObject()->className(), fieldName, "");
    }
    /*!
       Returns maximum value from given \a fieldName column that match \a condition.
     */
    double maximum(QString fieldName, ORMWhere condition) const
    {
        return ORMDatabase::adapter->calculation(ORMAbstractAdapter::Maximum, metaObject()->className(),
                                                 fieldName, condition.getWhereString());
    }
    /*!
       Returns minimum value from given \a fieldName column.
     */
    double minimum(QString fieldName) const
    {
        return ORMDatabase::adapter->calculation(ORMAbstractAdapter::Minimum, metaObject()->className(), fieldName, "");
    }
    /*!
       Returns minimum value from given \a fieldName column that match \a condition.
     */
    double minimum(QString fieldName, ORMWhere condition) const
    {
        return ORMDatabase::adapter->calculation(ORMAbstractAdapter::Minimum, metaObject()->className(),
                                                 fieldName, condition.getWhereString());
    }
    /*!
       Returns sum of values \a fieldName column.
     */
    double sum(QString fieldName) const
    {
        return ORMDatabase::adapter->calculation(ORMAbstractAdapter::Sum, metaObject()->className(), fieldName, "");
    }
    /*!
       Returns sum of values \a fieldName column that match \a condition.
     */
    double sum(QString fieldName, ORMWhere condition) const
    {
        return ORMDatabase::adapter->calculation(ORMAbstractAdapter::Sum, metaObject()->className(),
                                                 fieldName, condition.getWhereString());
    }
    /*!
       Returns list of objects that can be found for example where() method. \a T - class, a list of objects which will be returned
       Example:
       \code
       Student : public ORMObject
       {
        ...
       }
       ...
       Student model;
       QList<Student*> list = model.toList<Student>();
       \endcode
     */

protected:
    /*!
       Id is a primary key in any table. Immediately after creation object id = -1. After call save() id sets the positive value.
     */
    qlonglong id;
    template<class T>
    T* translateRecToObj(const QSqlRecord &record)
    {
        T *result = new T;
        for(int i = 0; i < record.count(); i++)
            if(record.fieldName(i) != "id")
                result->setProperty(record.fieldName(i).toLocal8Bit().data(), record.value(i));
            else
                result->setId(record.value(i).toInt());
        return result;
    }
};

#endif // ORMOBJECT_H
