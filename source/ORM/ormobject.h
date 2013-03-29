#ifndef ORMOBJECT_H
#define ORMOBJECT_H

#define ORM_PROPERTY(type, name) \
    Q_PROPERTY(type name READ get##name WRITE set##name) \
    private: \
    type name; \
    public: \
    type get##name(){ return name; } \
    void set##name(type input_##name) { name = input_##name; } \

#include <QObject>
#include <QMetaProperty>
#include <QSqlRecord>
#include "ormdatabase.h"
#include "ormwhere.h"

class ORMObject : public QObject
{
    Q_OBJECT

public:
    explicit ORMObject(QObject *parent = 0);
    bool createTable();
    bool dropTable();
    int getId();
    bool save();
    bool find(int id);
    bool first();
    bool last();
    bool findBy(QString fieldName, QVariant value);
    bool findBy(QHash<QString, QVariant> params);
    bool where(ORMWhere condition);
    //
    template<class T>
    QList<T*> toList();

protected:
    int id;

private:
    QList<QSqlRecord> m_records;
    void translateRecToThisObj(QSqlRecord record);

    template<class T>
    T* translateRecToObj(QSqlRecord record);
};

#endif // ORMOBJECT_H
