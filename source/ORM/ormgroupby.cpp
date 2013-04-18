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
            .arg(QString(this->m_groupString).remove(0, 9))
            .arg(QString(b.m_groupString).remove(0, 9));
    return result;
}

void ORMGroupBy::operator =(ORMGroupBy b)
{
    m_groupString = b.getGroupString();
}
