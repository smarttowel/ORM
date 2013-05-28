#ifndef POSTGRESQLADAPTER_H
#define POSTGRESQLADAPTER_H

#include "sqladapter.h"

class PostgreSQLAdapter : public SqlAdapter
{
public:
    PostgreSQLAdapter();
    bool createTable(const QString &tableName, const QHash<QString, QString> &info);
    bool createTableRelations(const QString &parent, Relation rel, const QString &child);
    int addRecord(const QString &tableName, const QHash<QString, QVariant> &info);

protected:
    void fillTableTypes();
};

#endif // POSTGRESQLADAPTER_H
