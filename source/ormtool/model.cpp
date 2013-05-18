#include "model.h"

Model::Model()
{
}

Model::Model(QString name)
{
    m_name = name;
}

QString Model::name() const
{
    return m_name;
}

void Model::addProperty(Property p)
{
    m_properties.append(p);
}

void Model::addHasOne(QString modelName)
{
    m_hasOneRelations.append(modelName);
}

void Model::addHasMany(QString modelName)
{
    m_hasManyRelations.append(modelName);
}

QStringList Model::hasOne() const
{
    return m_hasOneRelations;
}

QStringList Model::hasMany() const
{
    return m_hasManyRelations;
}

QList<Property> Model::properties() const
{
    return m_properties;
}

