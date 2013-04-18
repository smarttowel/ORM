#ifndef ORMOBJECT_H
#define ORMOBJECT_H

/*!
  \def ORM_PROPERTY(type, name)
  Declares property of model. Macros declare private property
  and public getter and setter.
 */

#define ORM_PROPERTY(type, name) \
    Q_PROPERTY(type name READ get##name WRITE set##name) \
    private: \
    type name; \
    public: \
    type get##name() const { return name; } \
    void set##name(type input_##name) { name = input_##name; } \

#include <QObject>
#include <QMetaProperty>
#include <QSqlRecord>
#include <QVector>
#include "ormdatabase.h"
#include "ormwhere.h"
#include "ormgroupby.h"

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


class ORMObject : public QObject
{
    Q_OBJECT

public:
    /*!
       Creates empty ORMObject with \a parent QObject.
     */
    explicit ORMObject(QObject *parent = 0);
    /*!
       Creates table associated with model.

       Returns true if table created, otherwise return false.
     */
    bool createTable() const;
    /*!
       Deletes table associated with model.

       Returns true if table deleted, otherwise return false.
     */
    bool dropTable() const;
    /*!
       Returns object id.
     */
    int getId() const;
    /*!
       Returns size of list objects.
     */
    int listSize();
    /*!
       Creates new record in table. Read all meta-property from model and save their into table.

       Returns true if success, otherwise return false.
     */
    bool save();
    /*!
       Updates existing record in table with object's \a id.

       Returns true if success, otherwise return false.
     */
    bool update();
    /*!
       Finds object by \a id.

       Returns true if object is found, otherwise return false.
     */
    bool find(int id);
    /*!
       Finds all records in table. List of objects you can get by calling toList().

       Returns true is success, otherwise retur false. If table is empty return false.
     */
    bool findAll(ORMGroupBy group = ORMGroupBy());
    /*!
       Finds first object in table.

       Returns true if object is found, otherwise return false.
     */
    bool first();
    /*!
       Finds last object in table.

       Returns true if object is found, otherwise return false.
     */
    bool last();
    /*!
       Finds object by some field and value. If there is more than one object, you can get them by toList().

       Returns true if object is found, otherwise return false.
     */
    bool findBy(const QString fieldName, const QVariant value, ORMGroupBy group = ORMGroupBy());
    /*!
       Finds objects by vector of values. If found more than one object, you can get them by toList().

       Returns true if object is found, otherwise return false.
     */
    bool findBy(const QString fieldName, const QVector<QVariant> &values, ORMGroupBy group = ORMGroupBy());
    /*!
       Finds object by many fields and values. If there is more than one object, you can get them by toList().
       \a params - QHash<fieldName, value>, of which will be searched.

       Returns true if object is found, otherwise return false.
     */
    bool findBy(const QHash<QString, QVariant> &params);
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
    bool where(ORMWhere condition, ORMGroupBy group = ORMGroupBy());
    /*!
       Returns true if table is empty, otherwise return false.
     */
    bool exists();
    /*!
       Returns true if object with given \a id exist, otherwise return false.
     */
    bool exists(int id);
    /*!
       Verification existence of objects with specified conditions.

       Returns true if exist at least one object, otherwise return false.
     */
    bool exists(ORMWhere condition);
    /*!
       Immediately updates object field in table.

       Returns true if property updated, otherwise return false.
     */
    bool updateProperty(QString fieldName, QVariant value);
    /*!
       Removes table's record with object's id.

       Returns true if success, otherwise return false.
     */
    bool remove();
    /*!
       Removes all objects, that match \a condition.

       Returns true if success, otherwise return false.
     */
    bool removeBy(ORMWhere condition);
    /*!
       Removes all records from table.

       Returns true is success, otherwise return false.
     */
    bool removeAll() const;
    /*!
       Returns number of objects in table.
     */
    int count() const;
    /*!
       Returns number of not null fields with given \a fieldName column.
     */
    int count(QString fieldName) const;
    /*!
       Returns number of objects that match \a condition.
     */
    int count(ORMWhere condition) const;
    /*!
       Returns average value from given \a fieldName column.
     */
    double average(QString fieldName) const;
    /*!
       Returns average value from given \a fieldName column that match \a condition.
     */
    double average(QString fieldName, ORMWhere condition) const;
    /*!
       Returns maximum value from given \a fieldName column.
     */
    double maximum(QString fieldName) const;
    /*!
       Returns maximum value from given \a fieldName column that match \a condition.
     */
    double maximum(QString fieldName, ORMWhere condition) const;
    /*!
       Returns minimum value from given \a fieldName column.
     */
    double minimum(QString fieldName) const;
    /*!
       Returns minimum value from given \a fieldName column that match \a condition.
     */
    double minimum(QString fieldName, ORMWhere condition) const;
    /*!
       Returns sum of values \a fieldName column.
     */
    double sum(QString fieldName) const;
    /*!
       Returns sum of values \a fieldName column that match \a condition.
     */
    double sum(QString fieldName, ORMWhere condition) const;
    //
    template<class T>
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
    QList<T*> toList();

protected:
    /*!
       Id is a primary key in any table. Immediately after creation object id = -1. After call save() id sets the positive value.
     */
    qlonglong id;

private:
    QList<QSqlRecord> m_records;
    void translateRecToThisObj(const QSqlRecord &record);

    template<class T>
    T* translateRecToObj(const QSqlRecord &record);
};

#endif // ORMOBJECT_H
