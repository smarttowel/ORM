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
    virtual bool createDatabase(QString name) = 0;
    virtual bool createTable(QString tableName, const QHash<QString, QString> &info) = 0;
    virtual bool dropTable(QString tableName) = 0;
    virtual bool dropDatabase(QString name) = 0;
    //
    QSqlError lastError();
    QString lastQuery();
    //
    virtual int addRecord(QString tableName, const QHash<QString, QVariant> &info) = 0;
    virtual bool updateRecord(QString tableName, qlonglong id, const QHash<QString, QVariant> &info) = 0;
    virtual QList<QSqlRecord> find(QString tableName, QString findString) = 0;
    virtual QSqlRecord first(QString tableName) = 0;
    virtual QSqlRecord last(QString tableName) = 0;

protected:
    QHash<QString, QString> m_tableTypes;
    QSqlQuery m_query;
    QString m_lastQuery;
    virtual void fillTableTypes() = 0;
};

#endif // ORMABSTRACTADAPTER_H
