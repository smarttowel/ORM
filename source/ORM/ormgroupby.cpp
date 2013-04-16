#include "ormgroupby.h"

ORMGroupBy::ORMGroupBy()
{
}

ORMGroupBy::ORMGroupBy(QString field)
{
    m_groupString = QString("GROUP BY %1")
            .arg(field);
}

QString ORMGroupBy::getGroupString()
{
    return m_groupString;
}

ORMGroupBy ORMGroupBy::operator &&(ORMGroupBy b)
{
    ORMGroupBy result;
    result.m_groupString = QString("GROUP BY %1, %2")
            .arg(this->m_groupString.right(this->m_groupString.size() - 9))
            .arg(b.m_groupString.right(b.m_groupString.size() - 9));
    return result;
}
