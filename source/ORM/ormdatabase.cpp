#include "ormdatabase.h"

ORMAbstractAdapter* ORMDatabase::adapter = 0;

ORMDatabase::ORMDatabase(QString driverName) :
    QSqlDatabase(QSqlDatabase::addDatabase(driverName))
{
    if(driverName == "QMYSQL")
        adapter = new MySqlAdapter();
}

ORMDatabase::ORMDatabase()
{
}

ORMDatabase ORMDatabase::addORMDatabase(QString driverName)
{
    return ORMDatabase(driverName);
}

bool ORMDatabase::createDatabase(QString name)
{
    open(userName(), password());
    return adapter->createDatabase(name);
}

QString ORMDatabase::lastQuery()
{
    return adapter->lastQuery();
}
