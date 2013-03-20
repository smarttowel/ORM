#ifndef ORMDATABASE_H
#define ORMDATABASE_H

#include <QSqlDatabase>
#include "mysqladapter.h"

class ORMDatabase : public QSqlDatabase
{
public:
    ORMDatabase(QString driverName);
    ORMDatabase();
    static ORMDatabase addORMDatabase(QString driverName);
    static ORMAbstractAdapter *adapter;
};

#endif // ORMDATABASE_H
