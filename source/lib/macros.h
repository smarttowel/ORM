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
    type get##name() const \
    { \
        return name; \
    } \
    void set##name(type input_##name)  \
    { \
        name = input_##name; \
        m_hasUnsavedChanges = true; \
        if(!m_propertiesForUpdate.contains(#name)) \
            m_propertiesForUpdate.append(#name); \
    }

/*!
   \def ORM_HAS_ONE(ClassName)
   Declares HAS_ONE relation. Example:
   \code
   class DriverLicense : public ORMObject<DriverLicense>
   {
       ...
   };

   class CarDriver : public ORMObject<CarDriver>
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
   Assigns \a object with model. If the child exists, it is updated with new values.\n
   \b NOTE: \a object must exists in table!

   <b>\a ClassName* create\a ClassName(const QHash<QString, QVariant> &values) \n</b>
   Creates new \a ClassName object from QHash<fieldName, value> \a values and call save() method. After assigns \a object with model
   and return pointer to it.

   <b>\a ClassName* get<em>ClassName</em>AfterIncludes() const \n </b>
   Returns pointer to \a ClassName that be loaded by includes(..) method.

 */

#define ORM_HAS_ONE(ClassName) \
    private: \
    QSqlRecord m_##ClassName##AfterIncludes; \
    Q_INVOKABLE void add##ClassName##AfterIncludes(QSqlRecord rec) \
    { \
        m_##ClassName##AfterIncludes = rec; \
    } \
    public: \
    ClassName* get##ClassName() \
    { \
        if(id < 0) \
            return 0; \
        QString whereString = QString("WHERE %1_id = %2") \
                                .arg(metaObject()->className()) \
                                .arg(id); \
        QList<QSqlRecord> list = ORMDatabase::adapter->find( #ClassName, "*", whereString); \
        if(list.isEmpty()) \
            return 0; \
        else \
            return translateRecToObj<ClassName>(list.first()); \
    } \
    void set##ClassName(const qlonglong childId) \
    { \
        if(childId < 0 || id < 0) \
            return; \
        QHash<QString, QVariant> hash; \
        hash.insert(QString(metaObject()->className()) + "_id", id); \
        ORMDatabase::adapter->updateRecord(#ClassName, childId, hash); \
    } \
    ClassName* create##ClassName(QHash<QString, QVariant> &values) \
    { \
        if(id < 0) \
            return 0; \
        values.insert(QString("%1_id").arg(metaObject()->className()), id); \
        int childId = ORMDatabase::adapter->addRecord(#ClassName, values); \
        return translateRecToObj<ClassName>(ORMDatabase::adapter->find(#ClassName, "*","WHERE id = " + QString::number(childId)).first()); \
    } \
    ClassName* get##ClassName##AfterIncludes() const\
    { \
        return translateRecToObj<ClassName>(m_##ClassName##AfterIncludes); \
    } \
    bool remove##ClassName(qlonglong id) \
    { \
        return ORMDatabase::adapter->setNull(#ClassName, QString("%1_id").arg(metaObject()->className()), \
            QString("WHERE id = '%1'") \
                .arg(id)); \
    }

/*!
   \def ORM_HAS_MANY(ClassName)
   Declares HAS_MANY relation. Example:
   \code
   class Car : public ORMObject<Car>
   {
       ...
   };

   class CarDriver : public ORMObject<CarDriver>
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
   Find child objects appropriate given \a where and returns them in accordance with \a group and \a order.

   <b>void get<em>ClassName</em>AfterIncludes() const \n </b>
   Returns list of \a ClassName that be loaded by includes(..) method.

 */

#define ORM_HAS_MANY(ClassName) \
    private: \
    QList<QSqlRecord> m_##ClassName##AfterIncludes; \
    Q_INVOKABLE void add##ClassName##AfterIncludes(QSqlRecord rec) \
    { \
        m_##ClassName##AfterIncludes.append(rec); \
    } \
    public: \
    QList<ClassName*> getAll##ClassName(ORMGroupBy group = ORMGroupBy(), ORMOrderBy order = ORMOrderBy()) \
    { \
        QList<ClassName*> result; \
        if(id < 0) \
            return result; \
        QString whereString = QString("WHERE %1_id = %2") \
                                .arg(metaObject()->className()) \
                                .arg(id); \
        QList<QSqlRecord> list = ORMDatabase::adapter->find( #ClassName, "*", whereString + " " + group.getGroupString() + \
                                                            " " + order.getOrderString()); \
        if(!list.isEmpty()) \
            for(int i = 0; i < list.size(); i++) \
                result.append(translateRecToObj<ClassName>(list.value(i))); \
        return result; \
    } \
    void add##ClassName(const qlonglong childId) \
    { \
        if(childId < 0 || id < 0) \
            return; \
        QHash<QString, QVariant> hash; \
        hash.insert(QString("%1_id").arg(metaObject()->className()), QString::number(id)); \
        ORMDatabase::adapter->updateRecord(#ClassName, childId, hash); \
    } \
    ClassName* create##ClassName(QHash<QString, QVariant> &values) \
    { \
        if(id < 0) \
            return 0; \
        values.insert(QString("%1_id").arg(metaObject()->className()), QString::number(id)); \
        int childId = ORMDatabase::adapter->addRecord(#ClassName, values); \
        return translateRecToObj<ClassName>(ORMDatabase::adapter->find(#ClassName, "*", "WHERE id = " + QString::number(childId)).first()); \
    } \
    QList<ClassName*> find##ClassName##Where(ORMWhere where, ORMGroupBy group = ORMGroupBy(), ORMOrderBy order = ORMOrderBy()) \
    { \
        QList<ClassName*> result; \
        if(id < 0) \
            return result; \
        QString whereString = QString("WHERE %1_id = %2") \
                                .arg(metaObject()->className()) \
                                .arg(id); \
        whereString += " AND " + where.getWhereString().remove(0, 6); \
        QList<QSqlRecord> list = ORMDatabase::adapter->find( #ClassName, "*",whereString + " " + group.getGroupString() + \
                                                            " " + order.getOrderString()); \
        if(!list.isEmpty()) \
            for(int i = 0; i < list.size(); i++) \
                result.append(translateRecToObj<ClassName>(list.value(i))); \
        return result; \
    } \
    QList<ClassName*> get##ClassName##AfterIncludes() const \
    { \
        QList<ClassName*> list; \
        for(int i = 0; i < m_##ClassName##AfterIncludes.size(); i++) \
            list.append(translateRecToObj<ClassName>(m_##ClassName##AfterIncludes.value(i))); \
        return list; \
    } \
    bool remove##ClassName(qlonglong id) \
    { \
        return ORMDatabase::adapter->setNull(#ClassName, QString("%1_id").arg(metaObject()->className()), \
            QString("WHERE id = '%1'") \
                .arg(id)); \
    } \
    bool removeAll##ClassName() \
    { \
        return ORMDatabase::adapter->setNull(#ClassName, QString("%1_id").arg(metaObject()->className()), \
            QString("WHERE %1_id = '%2'") \
                .arg(metaObject()->className()) \
                .arg(id)); \
    }

#endif // MACROS_H
