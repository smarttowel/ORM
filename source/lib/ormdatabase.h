#ifndef ORMDATABASE_H
#define ORMDATABASE_H

#include <QSqlDatabase>
#include "adapters/sqladapter.h"
#include "adapters/mysqladapter.h"
#include "adapters/sqliteadapter.h"
#include "adapters/postgresqladapter.h"

/*!
   \class ORMDatabase
   \brief The ORMDatabase is wrapper for QSqlDatabase

   The ORMDatabase provides interface for comfortable accessing a database with one connection.
   If you want use ORMObject models, you must create object of this class. Example:
   \code
   ORMDatabase myDatabase("QMYSQL");
   myDatabase.setUserName("root");
   myDatabase.open();
   myDatabase.createDatabase("MyDatabase");
   \endcode
   or
   \code
   ORMDatabase myDatabase;
   myDatabase = ORMDatabase::addORMDatabase("QMYSQL");
   myDatabase.setUserName("root");
   myDatabase.open();
   myDatabase.createDatabase("MyDatabase");
   \endcode
 */

class ORMDatabase : public QSqlDatabase
{
public:
    /*!
       Constructs empty ORMDatabase.
     */
    ORMDatabase();
    /*!
       Constructs ORMDatabase with given driverName. \a driverName must be supported Qt.
     */
    ORMDatabase(QString driverName);
    /*!
       Adapter builds various queries.
     */
    static ORMAbstractAdapter *adapter;
    /*!
       Constructs ORMDatabase with given driverName and returns it.
     */
    static ORMDatabase addORMDatabase(QString driverName);
    /*!
       Creates database with given \a name.

       Returns true if database created, otherwise return false.
     */
    bool createDatabase(QString name);
    /*!
       Deletes database with given \a name.

       Returns true if database deleted, otherwise return false
     */
    bool dropDatabase(QString name);
    /*!
       Returns last query to database.
     */
    QString lastQuery() const;
    void setLogDeep(OrmLogger::LogDeep deep);
};

#endif // ORMDATABASE_H
