#include "ormdatabase.h"

ORMDatabase::ORMDatabase(QString driverName) :
    QSqlDatabase(QSqlDatabase::addDatabase(driverName))
{
}

ORMDatabase::ORMDatabase()
{
}

ORMDatabase ORMDatabase::addORMDatabase(QString driverName)
{
    return ORMDatabase(driverName);
}
