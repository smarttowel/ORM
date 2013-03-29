#ifndef ORMWHERE_H
#define ORMWHERE_H

#include <QVariant>
#include <QString>

class ORMWhere
{
public:
    enum Condition
    {
        Equals,
        NotEquals,
        LessThan,
        LessOrEquals,
        GreaterThan,
        GreaterOrEquals,
        StartsWith,
        EndsWith,
        Contains,
        IsNull
    };
    ORMWhere();
    ORMWhere(QString fieldName, Condition con, QVariant value);
    void operator= (ORMWhere b);
    ORMWhere operator&& (ORMWhere b);
    ORMWhere operator|| (ORMWhere b);
    QString getWhereCondition();
private:
    QString m_whereCondition;
};

#endif // ORMWHERE_H
