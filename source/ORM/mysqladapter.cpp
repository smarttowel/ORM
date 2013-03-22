#include "mysqladapter.h"

MySqlAdapter::MySqlAdapter()
{
}

bool MySqlAdapter::createDatabase(QString name)
{
    m_lastQuery = QString("CREATE DATABASE %1;")
            .arg(name);
    bool result = m_query.exec(m_lastQuery);
    initDB(name);
    return result;
}

void MySqlAdapter::initDB(QString name)
{
    m_lastQuery = QString("USE %1;")
            .arg(name);
    m_query.exec(m_lastQuery);
}


