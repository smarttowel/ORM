#ifndef ORMGROUPBY_H
#define ORMGROUPBY_H

#include <QVector>
#include <QString>

class ORMGroupBy
{
public:
    ORMGroupBy();
    ORMGroupBy(QString field);
    QString getGroupString() const;
    ORMGroupBy operator&& (ORMGroupBy b);
    void operator= (ORMGroupBy b);

private:
    QString m_groupString;
};

#endif // ORMGROUPBY_H
