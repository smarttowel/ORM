#ifndef ORMDATABASE_H
#define ORMDATABASE_H

#include <QSqlDatabase>

class ORMDatabase : public QSqlDatabase
{
public:
    explicit ORMDatabase();
    
};

#endif // ORMDATABASE_H
