#ifndef ORMDATABASE_H
#define ORMDATABASE_H

#include <QSqlDatabase>

class ORMDatabase : public QSqlDatabase
{
public:
    ORMDatabase(QString driverName);
    ORMDatabase();
    static ORMDatabase addORMDatabase(QString driverName);
};

#endif // ORMDATABASE_H
