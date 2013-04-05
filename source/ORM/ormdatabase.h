#ifndef ORMDATABASE_H
#define ORMDATABASE_H

#include <QSqlDatabase>
#include "mysqladapter.h"

/*!
   \class ORMDatabase
   \brief The ORMDatabase is wrapper for QSqlDatabase

   The ORMDatabase provides interface for comfortable accessing a database with one connection.
   If you want use ORMObject models, you must create object of this class. Example:
   \code
   ORMDatabase myDatabase("QMYSQL");
   myDatabase.setUserName("root");
   myDatabase.createDatabase("MyDatabase");
   \endcode
   or
   \code
   ORMDatabase myDatabase;
   myDatabase = ORMDatabase::addORMDatabase("QMYSQL");
   myDatabase.setUserName("root");
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
       Constructs ORMDatabase with given driverName.
       \param driverName driver that support Qt
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
       Create database with given name
       \param name - database name
       \return true if database created, otherwise return false
     */
    bool createDatabase(QString name);
    /*!
       Delete database with given name
       \param name - database name
       \return true if database deleted, otherwise return false
     */
    bool dropDatabase(QString name);
    /*!
       Return last query to database.
       \return SQL query
     */
    QString lastQuery();
};

#endif // ORMDATABASE_H
