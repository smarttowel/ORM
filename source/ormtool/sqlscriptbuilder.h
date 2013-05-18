#ifndef SQLSCRIPTBUILDER_H
#define SQLSCRIPTBUILDER_H

#include <QList>
#include <QHash>
#include <QTextStream>
#include "model.h"
#include "property.h"
#include "logger.h"

class SqlScriptBuilder
{
public:
    SqlScriptBuilder();
    QString process(const QList<Model> &list);

private:
    QHash<QString, QString> m_tableTypes;
    QString createSqlScriptForTable(const Model &model) const;
    QString createRelationsForTable(const Model &model) const;
};

#endif // SQLSCRIPTBUILDER_H
