#ifndef ORMORDERBY_H
#define ORMORDERBY_H

#include <QString>

class ORMOrderBy
{
public:
    enum OrderType
    {
        Ascending,
        Descending
    };
    ORMOrderBy();
    ORMOrderBy(QString fieldName, OrderType type);
    QString getOrderString() const;
    ORMOrderBy& operator&& (ORMOrderBy b);
    void operator= (ORMOrderBy b);

private:
    QString m_orderString;
};

#endif // ORMORDERBY_H
