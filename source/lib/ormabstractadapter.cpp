#include "ormabstractadapter.h"

ORMAbstractAdapter::ORMAbstractAdapter()
{
}

QSqlError ORMAbstractAdapter::lastError() const
{
    return m_query.lastError();
}

QString ORMAbstractAdapter::lastQuery() const
{
    return m_lastQuery;
}
