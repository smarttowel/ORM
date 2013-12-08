#include "ormlogger.h"

OrmLogger::OrmLogger()
{
}

bool OrmLogger::exec(QSqlQuery &query, const QString &queryString)
{
    QTextStream stream(stdout);
    stream << '[' + QTime::currentTime().toString() + "] " + queryString << endl;
    if(query.exec(queryString))
        return true;
    else
    {
        stream << "Warning: " << query.lastError().text() << endl;
        return false;
    }
}
