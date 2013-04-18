#include "ormwhere.h"

ORMWhere::ORMWhere()
{
    m_whereCondition = "";
}

ORMWhere::ORMWhere(QString fieldName, ORMWhere::Condition con, QVariant value)
{
    m_whereCondition += QString("WHERE (%1 %3 '%2')")
            .arg(fieldName)
            .arg(value.toString());
    switch(con)
    {
    case ORMWhere::Equals:
        m_whereCondition = m_whereCondition
                .arg("=");
        break;
    case ORMWhere::NotEquals:
        m_whereCondition = m_whereCondition
                .arg("<>");
        break;
    case ORMWhere::LessThan:
        m_whereCondition = m_whereCondition
                .arg("<");
        break;
    case ORMWhere::LessOrEquals:
        m_whereCondition = m_whereCondition
                .arg("<=");
        break;
    case ORMWhere::GreaterThan:
        m_whereCondition = m_whereCondition
                .arg(">");
        break;
    case ORMWhere::GreaterOrEquals:
        m_whereCondition = m_whereCondition
                .arg(">=");
        break;
    case ORMWhere::StartsWith:
        m_whereCondition = QString("WHERE (%1 LIKE '%2%')")
                .arg(fieldName)
                .arg(value.toString());
        break;
    case ORMWhere::EndsWith:
        m_whereCondition = QString("WHERE (%1 LIKE '%%2')")
                .arg(fieldName)
                .arg(value.toString());
        break;
    case ORMWhere::Contains:
        m_whereCondition = QString("WHERE (%1 LIKE '%%2%')")
                .arg(fieldName)
                .arg(value.toString());
        break;
    case ORMWhere::IsNull:
        m_whereCondition = QString("WHERE (%1 IS NULL)")
                .arg(fieldName);
        break;
    default:
        m_whereCondition.clear();
    }
}

void ORMWhere::operator =(ORMWhere b)
{
    this->m_whereCondition = b.m_whereCondition;
}

ORMWhere ORMWhere::operator &&(ORMWhere b)
{
    ORMWhere result;
    result.m_whereCondition = QString("WHERE (%1 AND %2)")
            .arg(this->m_whereCondition.remove(0, 6))
            .arg(b.m_whereCondition.remove(0, 6));
    return result;
}

ORMWhere ORMWhere::operator ||(ORMWhere b)
{
    ORMWhere result;
    result.m_whereCondition = QString("WHERE (%1 OR %2)")
            .arg(this->m_whereCondition.remove(0, 6))
            .arg(b.m_whereCondition.remove(0, 6));
    return result;
}

QString ORMWhere::getWhereCondition() const
{
    return m_whereCondition;
}
