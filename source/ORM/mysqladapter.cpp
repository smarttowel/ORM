#include "mysqladapter.h"

MySqlAdapter::MySqlAdapter()
{
}

bool MySqlAdapter::createDatabase(QString name)
{
    m_lastQuery = QString("CREATE DATABASE %1;")
            .arg(name);
    return m_query.exec(m_lastQuery);
}


