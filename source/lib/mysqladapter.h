#ifndef MYSQLADAPTER_H
#define MYSQLADAPTER_H

#include "sqladapter.h"

class MySqlAdapter : public SqlAdapter
{
public:
    MySqlAdapter();
    bool createDatabase(const QString &name);
    bool createTable(const QString &tableName, const QHash<QString, QString> &info);
    bool createTableRelations(const QString &parent, Relation rel, const QString &child);

private:
    void initDB(const QString &name);
};

#endif // MYSQLADAPTER_H
