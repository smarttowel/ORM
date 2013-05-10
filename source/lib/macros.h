#ifndef MACROS_H
#define MACROS_H

/*! \file
*/

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

/*!
   \def ORM_HAS_ONE(ClassName)
   Declares HAS_ONE relation. Example:
   \code
   class DriverLicense : public ORMObject
   {
       ...
   };

   class CarDriver : public ORMObject
   {
       ...
       ORM_HAS_ONE(DriverLicense)

    public:
       CarDriver() {}
   };
   \endcode
   Macros generate next methods:

   <b>\a ClassName get\a ClassName() \n</b>
   Returns child \a ClassName, associated with model or 0, if no such child object.

   <b>void set\a ClassName(const \a Classname &object) \n</b>
   Assigns \a object with model. If the association already exists, this method removes the old and create a new one.
   Old relation removed.\n
   \b NOTE: \a object must exists in table!

   <b>\a ClassName* create\a ClassName(const QHash<QString, QVariant> &values) \n</b>
   Creates new \a ClassName object from QHash<fieldName, value> \a values and call save() method. After assigns \a object with model
   and return pointer to it. Old relation removed.

   <b>void remove\a ClassName() \n </b>
   Removes existing association, but not delete child object from table. If association not exist, does nothing.

 */

#define ORM_HAS_ONE(ClassName) \
    Q_CLASSINFO("HAS_ONE:", #ClassName) \
    public: \
    ClassName* get##ClassName() \
    { \
        if(id < 0) \
            return new ClassName; \
        QString tableName = QString("%1_HAS_ONE_" #ClassName) \
                                .arg(metaObject()->className()); \
        QString whereString = QString("WHERE id = (SELECT child_id FROM %1 WHERE parent_id = %2)") \
                                .arg(tableName) \
                                .arg(id); \
        QList<QSqlRecord> list = ORMDatabase::adapter->find( #ClassName, whereString); \
        if(list.isEmpty()) \
            return new ClassName; \
        else \
            return translateRecToObj<ClassName>(list.first()); \
    } \
    void set##ClassName(const ClassName &object) \
    { \
        if(object.getId() < 0 || id < 0) \
            return; \
        QString tableName = QString("%1_HAS_ONE_" #ClassName).arg(metaObject()->className()); \
        QString whereString = QString("WHERE parent_id = " + QString::number(id)); \
        ORMDatabase::adapter->remove(tableName, whereString); \
        QHash<QString, QVariant> hash; \
        hash.insert("parent_id", QString::number(id)); \
        hash.insert("child_id", QString::number(object.getId())); \
        ORMDatabase::adapter->addRecord(tableName, hash); \
    } \
    ClassName* create##ClassName(const QHash<QString, QVariant> &values) \
    { \
        if(id < 0) \
            return new ClassName; \
        int childId = ORMDatabase::adapter->addRecord(#ClassName, values); \
        QString tableName = QString("%1_HAS_ONE_" #ClassName) \
                                .arg(metaObject()->className()); \
        QHash<QString, QVariant> hash; \
        hash.insert("parent_id", QString::number(id)); \
        hash.insert("child_id", QString::number(childId)); \
        ORMDatabase::adapter->remove(tableName, "WHERE parent_id = " + QString::number(id)); \
        ORMDatabase::adapter->addRecord(tableName, hash); \
        return translateRecToObj<ClassName>(ORMDatabase::adapter->find(#ClassName, "WHERE id = " + QString::number(childId)).first()); \
    } \
    void remove##ClassName() \
    { \
        if(id < 0) \
            return; \
        QString tableName = QString("%1_HAS_ONE_" #ClassName) \
                                .arg(metaObject()->className()); \
        QString whereString = "WHERE parent_id = " + QString::number(id); \
        ORMDatabase::adapter->remove(tableName, whereString); \
    } \

/*!
   \def ORM_HAS_MANY(ClassName)
   Declares HAS_MANY relation. Example:
   \code
   class Car : public ORMObject
   {
       ...
   };

   class CarDriver : public ORMObject
   {
       ...
       ORM_HAS_ONE(DriverLicense)
       ORM_HAS_MANY(Car)

    public:
       CarDriver() {}
   };
   \endcode
   Macros generate next methods:

   <b> QList<\a ClassName*> getAll\a ClassName() \n</b>
   Returns all \a ClassName, associated with model or empty QList, if no such child object.

   <b>void add\a ClassName(const \a Classname &object) \n</b>
   Assigns \a object with model.\n
   \b NOTE: \a object must exists in table!

   <b>\a ClassName* create\a ClassName(const QHash<QString, QVariant> &values) \n</b>
   Creates new \a ClassName object from QHash<fieldName, value> \a values and call save() method. After assigns \a object with model
   and return pointer to it.

   <b>QList<\a ClassName*> find<em>ClassName</em>Where(ORMWhere where, ORMGroupBy group = ORMGroupBy(), ORMOrderBy order = ORMOrderBy()) \n</b>
   Find child objects appropriate given \a where and returns them in accordance with \group and \a order.

   <b>void remove\a ClassName(const \a ClassName &object) \n </b>
   Removes existing association, but not delete child object from table.\n
   \b NOTE: \a object must exists in table!

 */

#define ORM_HAS_MANY(ClassName) \
    Q_CLASSINFO("HAS_MANY:", #ClassName) \
    public: \
    QList<ClassName*> getAll##ClassName(ORMGroupBy group = ORMGroupBy(), ORMOrderBy order = ORMOrderBy()) \
    { \
        QList<ClassName*> result; \
        if(id < 0) \
            return result; \
        QString tableName = QString("%1_HAS_MANY_" #ClassName) \
                                .arg(metaObject()->className()); \
        QString whereString = QString("WHERE id IN (SELECT child_id FROM %1 WHERE parent_id = %2)") \
                                .arg(tableName) \
                                .arg(id); \
        QList<QSqlRecord> list = ORMDatabase::adapter->find( #ClassName, whereString + " " + group.getGroupString() + \
                                                            " " + order.getOrderString()); \
        if(!list.isEmpty()) \
            for(int i = 0; i < list.size(); i++) \
                result.append(translateRecToObj<ClassName>(list.value(i))); \
        return result; \
    } \
    void add##ClassName(const ClassName &object) \
    { \
        if(object.getId() < 0 || id < 0) \
            return; \
        QString tableName = QString("%1_HAS_MANY_" #ClassName).arg(metaObject()->className()); \
        QString whereString = QString("WHERE parent_id = " + QString::number(id)); \
        QHash<QString, QVariant> hash; \
        hash.insert("parent_id", QString::number(id)); \
        hash.insert("child_id", QString::number(object.getId())); \
        ORMDatabase::adapter->addRecord(tableName, hash); \
    } \
    ClassName* create##ClassName(const QHash<QString, QVariant> &values) \
    { \
        if(id < 0) \
            return new ClassName; \
        int childId = ORMDatabase::adapter->addRecord(#ClassName, values); \
        QString tableName = QString("%1_HAS_MANY_" #ClassName) \
                                .arg(metaObject()->className()); \
        QHash<QString, QVariant> hash; \
        hash.insert("parent_id", QString::number(id)); \
        hash.insert("child_id", QString::number(childId)); \
        ORMDatabase::adapter->addRecord(tableName, hash); \
        return translateRecToObj<ClassName>(ORMDatabase::adapter->find(#ClassName, "WHERE id = " + QString::number(childId)).first()); \
    } \
    QList<ClassName*> find##ClassName##Where(ORMWhere where, ORMGroupBy group = ORMGroupBy(), ORMOrderBy order = ORMOrderBy()) \
    { \
        QList<ClassName*> result; \
        if(id < 0) \
            return result; \
        QString tableName = QString("%1_HAS_MANY_" #ClassName) \
                                .arg(metaObject()->className()); \
        QString whereString = QString("WHERE id IN (SELECT child_id FROM %1 WHERE parent_id = %2)") \
                                .arg(tableName) \
                                .arg(id); \
        whereString += " AND " + where.getWhereString().remove(0, 6); \
        QList<QSqlRecord> list = ORMDatabase::adapter->find( #ClassName, whereString + " " + group.getGroupString() + \
                                                            " " + order.getOrderString()); \
        if(!list.isEmpty()) \
            for(int i = 0; i < list.size(); i++) \
                result.append(translateRecToObj<ClassName>(list.value(i))); \
        return result; \
    } \
    void remove##ClassName(const ClassName &object) \
    { \
        if(object.getId() < 0 || id < 0) \
            return; \
        QString tableName = QString("%1_HAS_MANY_" #ClassName) \
                                .arg(metaObject()->className()); \
        QString whereString = QString("WHERE parent_id = %1 && child_id = %2") \
                                .arg(id) \
                                .arg(object.getId()); \
        ORMDatabase::adapter->remove(tableName, whereString); \
    } \

#endif // MACROS_H
