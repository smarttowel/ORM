#ifndef ORMLOGGER_H
#define ORMLOGGER_H

#include <QTextStream>
#include <QSqlQuery>
#include <QTime>
#include <QSqlError>

class OrmLogger
{
public:
    OrmLogger();
    bool exec(QSqlQuery &query, const QString &queryString);

};

#endif // ORMLOGGER_H
