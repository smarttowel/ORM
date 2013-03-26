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
    virtual bool createTable(QString tableName, QHash<QString, QString> info) = 0;
    QSqlError lastError();
    QString lastQuery();
    virtual int addRecord(QString tableName, QHash<QString, QVariant> info) = 0;

protected:
    QHash<QString, QString> m_tableTypes;
    QSqlQuery m_query;
    QString m_lastQuery;
    virtual void fillTableTypes() = 0;
};

#endif // ORMABSTRACTADAPTER_H
