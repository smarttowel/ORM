#ifndef ORMDATABASE_H
#define ORMDATABASE_H

#include <QSqlDatabase>
#include "mysqladapter.h"

class ORMDatabase : public QSqlDatabase
{
public:
    ORMDatabase(QString driverName);
    ORMDatabase();
    static ORMAbstractAdapter *adapter;
    static ORMDatabase addORMDatabase(QString driverName);
    bool createDatabase(QString name);
    bool dropDatabase(QString name);
    QString lastQuery();
};

#endif // ORMDATABASE_H
