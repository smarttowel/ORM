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
    bool setDriverName(const QString &driverName);

private:
    QHash<QString, QString> m_tableTypes;
    QString m_driverName;
    QString createSqlScriptForTableMySql(const Model &model) const;
    QString createRelationForTableMySql(const Model &model) const;
    QString createSqlScriptForTablePostgreSql(const Model &model) const;
    QString createRelationForTablePostgreSql(const Model &model) const;
    QString createSqlScriptForTableSqlite(const Model &model) const;
    QString createRelationForTableSqlite(const Model &model) const;
    void fillTableTypesMySql();
    void fillTableTypesPostgreSql();
};

#endif // SQLSCRIPTBUILDER_H
