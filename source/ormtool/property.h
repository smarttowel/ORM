#ifndef PROPERTY_H
#define PROPERTY_H

#include <QString>

class Property
{
public:
    Property(QString type, QString name);
    QString getType();
    QString getName();

private:
    QString m_type;
    QString m_name;
};

#endif // PROPERTY_H
