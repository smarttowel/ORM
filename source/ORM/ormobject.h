#ifndef ORMOBJECT_H
#define ORMOBJECT_H

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
#include "ormdatabase.h"
#include "ormwhere.h"

class ORMObject : public QObject
{
    Q_OBJECT

public:
    explicit ORMObject(QObject *parent = 0);
    bool createTable() const;
    bool dropTable() const;
    int getId() const;
    bool save();
    bool find(int id);
    bool first();
    bool last();
    bool findBy(const QString fieldName, const QVariant value);
    bool findBy(const QHash<QString, QVariant> &params);
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
