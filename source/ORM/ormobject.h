#ifndef ORMOBJECT_H
#define ORMOBJECT_H

/*!
  \def ORM_PROPERTY(type, name)
  Declared property of model. Macros declare private property
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
       \param parent QObject
     */
    explicit ORMObject(QObject *parent = 0);
    /*!
       Create table associated with model
       \return true if table created, otherwise return false
     */
    bool createTable() const;
    /*!
       Delete table associated with model
       \return true if table deleted, otherwise return false
     */
    bool dropTable() const;
    /*!
       Return model id.
       \return Id contains current object id. Immediately after creation id = -1.
     */
    int getId() const;
    /*!
       Read all meta-property from model and save their into table.
       If object's id less than 0 - create new object, otherwise updated existing object.
       \return true if success, otherwise return false
     */
    bool save();
    bool update();
    /*!
       Find object by id
       \param id - existing object id
       \return true if object is found, otherwise return false
     */
    bool find(int id);
    bool findAll();
    /*!
        Find first object in table
        \return true if object is found, otherwise return false
     */
    bool first();
    /*!
       Find last object in table
       \return true if object is found, otherwise return false
     */
    bool last();
    /*!
     * Find object by some field and value. If there is more than
       one object, you can get them by toList().
       \param fieldName - field name
       \param value - value, of which will be searched
       \return true if object is found, otherwise return false
     */
    bool findBy(const QString fieldName, const QVariant value);
    /*!
       Find objects by vector of values. If found more than one object, you can get them by toList().
       \param fieldName - field name for search
       \param values - vector of values for search
       \return true if object is found, otherwise return false
     */
    bool findBy(const QString fieldName, const QVector<QVariant> &values);
    /*!
       Find object by many fields and values. If there is more than
       one object, you can get them by toList().
       \param params - Hash<fieldName, value>, of which will be searched
       \return true if object is found, otherwise return false
     */
    bool findBy(const QHash<QString, QVariant> &params);
    /*!
       Find object with use WHERE expression. If there is more than
       one object, you can get them by toList(). For ORMWhere defined && and || operators,
       so you use next code:
       \code
       Model.where(ORMWhere(...) || ORMWhere(...));
       \endcode
       or
       \code
       Model.where(ORMWhere(...) && ORMWhere(...));
       \endcode
       \param condition - WHERE expression
       \return true if object is found, otherwise return false
     */
    bool where(ORMWhere condition);
    /*!
       Immediately updates object field in table.
       \param fieldName - field name, value of which will be updated
       \param value - value for write to table
       \return true if property updated, otherwise return false
     */
    bool updateProperty(QString fieldName, QVariant value);
    bool remove();
    bool removeBy(ORMWhere condition);
    bool removeAll();
    //
    template<class T>
    /*!
       Return list of objects that can be found for example where() method.
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
       \param T - class, a list of objects which will be returned
       \return list of objects
     */
    QList<T*> toList();

protected:
    /*!
     * Id is a primary key in any table.
     */
    qlonglong id;

private:
    QList<QSqlRecord> m_records;
    void translateRecToThisObj(const QSqlRecord &record);

    template<class T>
    T* translateRecToObj(const QSqlRecord &record);
};

#endif // ORMOBJECT_H
