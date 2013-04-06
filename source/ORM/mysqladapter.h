#ifndef MYSQLADAPTER_H
#define MYSQLADAPTER_H

#include <QVariant>
#include <QSqlRecord>
#include "ormabstractadapter.h"

class MySqlAdapter : public ORMAbstractAdapter
{
public:
    MySqlAdapter();
    bool createDatabase(QString name);
    bool createTable(QString tableName, const QHash<QString, QString> &info);
    bool dropTable(QString tableName);
    bool dropDatabase(QString name);
    int addRecord(QString tableName, const QHash<QString, QVariant> &info);
    bool updateRecord(QString tableName, qlonglong id, const QHash<QString, QVariant> &info);
    QList<QSqlRecord> find(QString tableName, QString findString);
    QList<QSqlRecord> findAll(QString tableName);
    virtual QSqlRecord first(QString tableName);
    virtual QSqlRecord last(QString tableName);

protected:
    void fillTableTypes();

private:
    void initDB(QString name);
};

#endif // MYSQLADAPTER_H
