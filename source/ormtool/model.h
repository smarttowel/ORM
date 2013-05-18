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
    QString name() const;
    void addProperty(Property p);
    void addHasOne(QString modelName);
    void addHasMany(QString modelName);
    QStringList hasOne() const;
    QStringList hasMany() const;
    QList<Property> properties() const;

private:
    QString m_name;
    QList<Property> m_properties;
    QStringList m_hasOneRelations;
    QStringList m_hasManyRelations;
};

#endif // MODEL_H
