#include "ormlogger.h"

OrmLogger::OrmLogger() :
    m_logDeep(ldAll)
{
}

bool OrmLogger::exec(QSqlQuery &query, const QString &queryString)
{
    QTextStream stream(stdout);
    bool result = query.exec(queryString);
    switch(m_logDeep)
    {
    case ldAll:
    {
        stream << '[' + QTime::currentTime().toString() + "] " + queryString << endl;
        if(!result)
            stream << "Warning: " << query.lastError().text() << endl;
        break;
    }
    case ldWarningsOnly:
    {
        if(!result)
        {
            stream << '[' + QTime::currentTime().toString() + "] " + queryString << endl;
            stream << "Warning: " << query.lastError().text() << endl;
        }
        break;
    }
    case ldNone: {}
    }
    return result;
}

void OrmLogger::setLogDeep(OrmLogger::LogDeep deep)
{
    m_logDeep = deep;
}
