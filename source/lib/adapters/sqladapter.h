#ifndef SQLADAPTER_H
#define SQLADAPTER_H

#include <QVariant>
#include <QSqlRecord>
#include <QStringList>
#include <QSqlField>
#include "ormabstractadapter.h"

class SqlAdapter : public ORMAbstractAdapter
{
public:
    SqlAdapter();
    bool createDatabase(const QString &name);
    bool createTable(const QString &tableName, const QHash<QString, QString> &info);
    bool createTableRelations(const QString &parent, Relation rel, const QString &child);
    bool dropTable(const QString &tableName);
    bool dropDatabase(const QString &name);
    int addRecord(const QString &tableName, const QHash<QString, QVariant> &info);
    bool updateRecord(const QString &tableName, const qlonglong id, const QHash<QString, QVariant> &info);
    QList<QSqlRecord> find(const QString &tableName, const QString &fieldName, const QString &params);
    QSqlRecord first(const QString &tableName);
    QSqlRecord last(const QString &tableName);
    bool setNull(const QString &tableName, const QString &fieldName, const QString &params);
    bool remove(const QString &tableName, const QString &params);
    int count(const QString &tableName, const QString &arg);
    int countBy(const QString &tableName, const QString &params);
    double calculation(Calculation func, const QString &tableName, const QString &fieldName, const QString &params);
    QHash<QString, QList<QSqlRecord> > includes(const QString &parentModel, const QStringList &childModels, const QString &params);

protected:
    void fillTableTypes();
};

#endif // SQLADAPTER_H
