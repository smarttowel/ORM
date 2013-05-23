#include "ormwhere.h"

ORMWhere::ORMWhere()
{
}

ORMWhere::ORMWhere(QString fieldName, ORMWhere::Condition con, QVariant value)
{
    m_whereString += QString("(%1 %3 '%2')")
            .arg(fieldName)
            .arg(value.toString());
    switch(con)
    {
    case ORMWhere::Equals:
        m_whereString = m_whereString
                .arg("=");
        break;
    case ORMWhere::NotEquals:
        m_whereString = m_whereString
                .arg("<>");
        break;
    case ORMWhere::LessThan:
        m_whereString = m_whereString
                .arg("<");
        break;
    case ORMWhere::LessOrEquals:
        m_whereString = m_whereString
                .arg("<=");
        break;
    case ORMWhere::GreaterThan:
        m_whereString = m_whereString
                .arg(">");
        break;
    case ORMWhere::GreaterOrEquals:
        m_whereString = m_whereString
                .arg(">=");
        break;
    case ORMWhere::StartsWith:
        m_whereString = QString("(%1 LIKE '%2%')")
                .arg(fieldName)
                .arg(value.toString());
        break;
    case ORMWhere::EndsWith:
        m_whereString = QString("(%1 LIKE '%%2')")
                .arg(fieldName)
                .arg(value.toString());
        break;
    case ORMWhere::Contains:
        m_whereString = QString("(%1 LIKE '%%2%')")
                .arg(fieldName)
                .arg(value.toString());
        break;
    case ORMWhere::IsNull:
        m_whereString = QString("(%1 IS NULL)")
                .arg(fieldName);
        break;
    default:
        m_whereString.clear();
    }
}

ORMWhere::ORMWhere(QString fieldName, QList<QVariant> values)
{
    m_whereString = QString("(%1 IN (")
            .arg(fieldName);
    for(int i = 0; i < values.size(); i++)
        m_whereString += '\'' + values.value(i).toString() + "', ";
    m_whereString.resize(m_whereString.size() - 2);
    m_whereString += "))";
}

void ORMWhere::operator =(ORMWhere b)
{
    this->m_whereString = b.m_whereString;
}

ORMWhere& ORMWhere::operator &&(ORMWhere b)
{
    m_whereString = QString("(%1 AND %2)")
            .arg(m_whereString)
            .arg(b.m_whereString);
    return *this;
}

ORMWhere& ORMWhere::operator ||(ORMWhere b)
{
    m_whereString = QString("(%1 OR %2)")
            .arg(m_whereString)
            .arg(b.m_whereString);
    return *this;
}

QString ORMWhere::getWhereString() const
{
    if(m_whereString.isEmpty())
        return m_whereString;
    else
        return "WHERE " + m_whereString;
}
