#ifndef ORMABSTRACTADAPTER_H
#define ORMABSTRACTADAPTER_H

#include <QObject>
#include <QSqlQuery>
#include <QSqlError>

class ORMAbstractAdapter
{
public:
    ORMAbstractAdapter();
    virtual bool createDatabase(QString name) = 0;
    QSqlError lastError();
    QString lastQuery();


protected:
    QSqlQuery m_query;
    QString m_lastQuery;
};

#endif // ORMABSTRACTADAPTER_H
