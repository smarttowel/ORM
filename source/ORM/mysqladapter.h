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
    bool createTable(QString tableName, QHash<QString, QString> info);
    bool dropTable(QString tableName);
    bool dropDatabase(QString name);
    int addRecord(QString tableName, QHash<QString, QVariant> info);
    QList<QSqlRecord> find(QString tableName, QString findString);

protected:
    void fillTableTypes();

private:
    void initDB(QString name);
};

#endif // MYSQLADAPTER_H
