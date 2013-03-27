#ifndef MYSQLADAPTER_H
#define MYSQLADAPTER_H

#include <QVariant>
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

protected:
    void fillTableTypes();

private:
    void initDB(QString name);
};

#endif // MYSQLADAPTER_H
