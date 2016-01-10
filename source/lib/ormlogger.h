#ifndef ORMLOGGER_H
#define ORMLOGGER_H

#include <QTextStream>
#include <QSqlQuery>
#include <QTime>
#include <QSqlError>

class OrmLogger
{
public:
    enum LogDeep
    {
        ldAll,
        ldWarningsOnly,
        ldNone
    };
    OrmLogger();
    bool exec(QSqlQuery query);
    bool exec(QSqlQuery &query, const QString &queryString);
    void setLogDeep(OrmLogger::LogDeep deep);

private:
    LogDeep m_logDeep;
};

#endif // ORMLOGGER_H
