#include "mysqladapter.h"

MySqlAdapter::MySqlAdapter()
{
}

bool MySqlAdapter::createDatabase(const QString &name)
{
    m_lastQuery = QString("CREATE DATABASE %1;")
            .arg(name);
    bool result = m_query.exec(m_lastQuery);
    if(result)
        initDB(name);
    return result;
}

void MySqlAdapter::initDB(const QString &name)
{
    m_query.exec(QString("USE %1;")
                 .arg(name));
}
