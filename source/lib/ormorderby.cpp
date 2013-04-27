#include "ormorderby.h"

ORMOrderBy::ORMOrderBy()
{
}

ORMOrderBy::ORMOrderBy(QString fieldName, ORMOrderBy::OrderType type)
{
    m_orderString = fieldName;
    switch(type)
    {
    case Ascending:
        m_orderString += " ASC";
        break;
    case Descending:
        m_orderString += " DESC";
        break;
    default:
        m_orderString.clear();
    }
}

QString ORMOrderBy::getOrderString() const
{
    if(m_orderString.isEmpty())
        return m_orderString;
    else
        return "ORDER BY " + m_orderString;
}

ORMOrderBy& ORMOrderBy::operator &&(ORMOrderBy b)
{
    m_orderString += ", " + b.m_orderString;
    return *this;
}

void ORMOrderBy::operator =(ORMOrderBy b)
{
    this->m_orderString = b.m_orderString;
}
