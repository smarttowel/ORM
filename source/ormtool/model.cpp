#include "model.h"

Model::Model()
{
}

Model::Model(QString name)
{
    m_name = name;
}

QString Model::name()
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

QStringList Model::hasOne()
{
    return m_hasOneRelations;
}

QStringList Model::hasMany()
{
    return m_hasManyRelations;
}

QList<Property> Model::properties()
{
    return m_properties;
}

