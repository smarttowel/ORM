#include "ormgroupby.h"

ORMGroupBy::ORMGroupBy()
{
}

ORMGroupBy::ORMGroupBy(QString field)
{
    m_groupString = field;
}

QString ORMGroupBy::getGroupString() const
{
    if(m_groupString.isEmpty())
        return m_groupString;
    else
        return "GROUP BY " + m_groupString;
}

ORMGroupBy& ORMGroupBy::operator &&(ORMGroupBy b)
{
    m_groupString += ", " + b.m_groupString;
    return *this;
}

void ORMGroupBy::operator =(ORMGroupBy b)
{
    m_groupString = b.m_groupString;
}
