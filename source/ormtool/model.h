#ifndef MODEL_H
#define MODEL_H

#include <QList>
#include <QStringList>
#include "property.h"

class Model
{
public:
    Model();
    Model(QString name);
    QString name();
    void addProperty(Property p);
    void addHasOne(QString modelName);
    void addHasMany(QString modelName);
    QStringList hasOne();
    QStringList hasMany();
    QList<Property> properties();

private:
    QString m_name;
    QList<Property> m_properties;
    QStringList m_hasOneRelations;
    QStringList m_hasManyRelations;
};

#endif // MODEL_H
