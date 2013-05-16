#ifndef SQLSCRIPTBUILDER_H
#define SQLSCRIPTBUILDER_H

#include <QList>
#include <QHash>
#include "model.h"
#include "property.h"

class SqlScriptBuilder
{
public:
    SqlScriptBuilder();
    QString process(QList<Model> list);
    QString createSqlScriptForTable(Model model);

private:
    QHash<QString, QString> m_tableTypes;
};

#endif // SQLSCRIPTBUILDER_H
