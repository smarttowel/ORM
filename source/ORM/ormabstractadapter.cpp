#include "ormabstractadapter.h"

ORMAbstractAdapter::ORMAbstractAdapter()
{
}

QSqlError ORMAbstractAdapter::lastError()
{
    return m_query.lastError();
}

QString ORMAbstractAdapter::lastQuery()
{
    return m_lastQuery;
}
