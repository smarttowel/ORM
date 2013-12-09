#ifndef ORMABSTRACTADAPTER_H
#define ORMABSTRACTADAPTER_H

#include <QObject>
#include <QHash>
#include <QSqlQuery>
#include <QSqlError>
#include "ormlogger.h"

class ORMAbstractAdapter
{
public:
    ORMAbstractAdapter();
    enum Calculation
    {
        Average,
        Maximum,
        Minimum,
        Sum
    };
    enum Relation
    {
        HasOne,
        HasMany
    };
    virtual bool createDatabase(const QString &name) = 0;
    virtual bool createTable(const QString &tableName, const QHash<QString, QString> &info) = 0;
    virtual bool createTableRelations(const QString &parent, Relation rel, const QString &child) = 0;
    virtual bool dropTable(const QString &tableName) = 0;
    virtual bool dropDatabase(const QString &name) = 0;
    //
    QSqlError lastError() const;
    QString lastQuery() const;
    //
    virtual int addRecord(const QString &tableName, const QHash<QString, QVariant> &info) = 0;
    virtual bool updateRecord(const QString &tableName, const qlonglong id, const QHash<QString, QVariant> &info) = 0;
    virtual QList<QSqlRecord> find(const QString &tableName, const QString &fieldName, const QString &params) = 0;
    virtual QSqlRecord first(const QString &tableName) = 0;
    virtual QSqlRecord last(const QString &tableName) = 0;
    virtual bool setNull(const QString &tableName, const QString &fieldName, const QString &params) = 0;
    virtual bool remove(const QString &tableName, const QString &params) = 0;
    virtual int count(const QString &tableName, const QString &arg) = 0;
    virtual int countBy(const QString &tableName, const QString &params) = 0;
    virtual double calculation(Calculation func, const QString &tableName, const QString &fieldName, const QString &params) = 0;
    virtual QHash<QString, QList<QSqlRecord> > includes(const QString &parentModel, const QStringList &childModels, const QString &params) = 0;
    OrmLogger m_logger;

protected:
    QHash<QString, QString> m_tableTypes;
    QSqlQuery m_query;
    QString m_lastQuery;
    virtual void fillTableTypes() = 0;
};

#endif // ORMABSTRACTADAPTER_H
