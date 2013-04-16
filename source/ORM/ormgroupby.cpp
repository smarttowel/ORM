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
