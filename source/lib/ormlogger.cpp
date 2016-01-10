#include "ormlogger.h"

OrmLogger::OrmLogger() :
    m_logDeep(ldAll)
{
}

bool OrmLogger::exec(QSqlQuery query)
{
    QTextStream stream(stdout);
    bool result = query.exec();
    switch(m_logDeep)
    {
    case ldAll:
    {
        stream << '[' + QTime::currentTime().toString() + "] " + query.lastQuery() << endl;
        if(!result)
            stream << "Warning: " << query.lastError().text() << endl;
        break;
    }
    case ldWarningsOnly:
    {
        if(!result)
        {
            stream << '[' + QTime::currentTime().toString() + "] " + query.lastQuery() << endl;
            stream << "Warning: " << query.lastError().text() << endl;
        }
        break;
    }
    case ldNone: {}
    }
    return result;
}

bool OrmLogger::exec(QSqlQuery &query, const QString &queryString)
{
    query.prepare(queryString);
    return exec(query);
}

void OrmLogger::setLogDeep(OrmLogger::LogDeep deep)
{
    m_logDeep = deep;
}
