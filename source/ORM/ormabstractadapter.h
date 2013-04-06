#ifndef ORMABSTRACTADAPTER_H
#define ORMABSTRACTADAPTER_H

#include <QObject>
#include <QHash>
#include <QSqlQuery>
#include <QSqlError>

class ORMAbstractAdapter
{
public:
    ORMAbstractAdapter();
    virtual bool createDatabase(const QString name) = 0;
    virtual bool createTable(const QString tableName, const QHash<QString, QString> &info) = 0;
    virtual bool dropTable(const QString tableName) = 0;
    virtual bool dropDatabase(const QString name) = 0;
    //
    QSqlError lastError();
    QString lastQuery();
    //
    virtual int addRecord(const QString tableName, const QHash<QString, QVariant> &info) = 0;
    virtual bool updateRecord(const QString tableName, const qlonglong id, const QHash<QString, QVariant> &info) = 0;
    virtual QList<QSqlRecord> find(const QString tableName, const QString findString) = 0;
    virtual QList<QSqlRecord> findAll(const QString tableName) = 0;
    virtual QSqlRecord first(const QString tableName) = 0;
    virtual QSqlRecord last(const QString tableName) = 0;

protected:
    QHash<QString, QString> m_tableTypes;
    QSqlQuery m_query;
    QString m_lastQuery;
    virtual void fillTableTypes() = 0;
};

#endif // ORMABSTRACTADAPTER_H
