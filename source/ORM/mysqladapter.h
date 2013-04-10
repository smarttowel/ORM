#ifndef MYSQLADAPTER_H
#define MYSQLADAPTER_H

#include <QVariant>
#include <QSqlRecord>
#include "ormabstractadapter.h"

class MySqlAdapter : public ORMAbstractAdapter
{
public:
    MySqlAdapter();
    bool createDatabase(const QString name);
    bool createTable(const QString tableName, const QHash<QString, QString> &info);
    bool dropTable(const QString tableName);
    bool dropDatabase(const QString name);
    int addRecord(const QString tableName, const QHash<QString, QVariant> &info);
    bool updateRecord(const QString tableName, const qlonglong id, const QHash<QString, QVariant> &info);
    QList<QSqlRecord> find(const QString tableName, const QString findString);
    QList<QSqlRecord> findAll(const QString tableName);
    virtual QSqlRecord first(const QString tableName);
    virtual QSqlRecord last(const QString tableName);
    virtual bool remove(const QString tableName, const QString whereString);
    virtual bool removeAll(const QString tableName);
    virtual int count(const QString tableName, const QString arg);
    virtual int countBy(const QString tableName, const QString whereString);
    virtual double average(const QString tableName, const QString fieldName);
    virtual double maximum(const QString tableName, const QString fieldName);

protected:
    void fillTableTypes();

private:
    void initDB(QString name);
};

#endif // MYSQLADAPTER_H
