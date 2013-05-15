#include "property.h"

Property::Property()
{
}

Property::Property(QString type, QString name)
{
    m_type = type;
    m_name = name;
}

QString Property::getType()
{
    return m_type;
}

QString Property::getName()
{
    return m_name;
}
