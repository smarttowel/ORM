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

#define ORM_HAS_ONE(ClassName) \
    Q_CLASSINFO("HAS_ONE:", #ClassName) \
    public: \
    ClassName* get##ClassName() \
    { \
        if(id < 0) \
            return 0; \
        QString tableName = QString("%1_HAS_ONE_" #ClassName) \
                                .arg(metaObject()->className()); \
        QString whereString = QString("WHERE id = (SELECT child_id FROM %1 WHERE parent_id = %2)") \
                                .arg(tableName) \
                                .arg(id); \
        QList<QSqlRecord> list = ORMDatabase::adapter->find( #ClassName, whereString); \
        if(list.isEmpty()) \
            return 0; \
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
            return 0; \
        int childId = ORMDatabase::adapter->addRecord(#ClassName, values); \
        QString tableName = QString("%1_HAS_ONE_" #ClassName) \
                                .arg(metaObject()->className()); \
        QHash<QString, QVariant> hash; \
        hash.insert("parent_id", QString::number(id)); \
        hash.insert("child_id", QString::number(childId)); \
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
            return 0; \
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
