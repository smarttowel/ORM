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
    return adapter->createDatabase(name);
}

bool ORMDatabase::dropDatabase(QString name)
{
    return adapter->dropDatabase(name);
}

QString ORMDatabase::lastQuery() const
{
    return adapter->lastQuery();
}
