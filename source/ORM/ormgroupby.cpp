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

ORMGroupBy ORMGroupBy::operator &&(ORMGroupBy b)
{
    ORMGroupBy result;
    result.m_groupString = this->m_groupString + ", " + b.m_groupString;
    return result;
}

void ORMGroupBy::operator =(ORMGroupBy b)
{
    m_groupString = b.m_groupString;
}
