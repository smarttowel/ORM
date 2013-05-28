#include "sqlscriptbuilder.h"

SqlScriptBuilder::SqlScriptBuilder()
{
}

QString SqlScriptBuilder::process(const QList<Model> &list)
{
    QString script;
    if(m_driverName == "QMYSQL")
    {
        fillTableTypesMySql();
        for(int i = 0; i < list.size(); i++)
            script += createSqlScriptForTableMySql(list.value(i));
        for(int i = 0; i < list.size(); i++)
            script += createRelationForTableMySql(list.value(i));
    }
    else if(m_driverName == "QPSQL")
    {
        fillTableTypesPostgreSql();
        for(int i = 0; i < list.size(); i++)
            script += createSqlScriptForTablePostgreSql(list.value(i));
        for(int i = 0; i < list.size(); i++)
            script += createRelationForTablePostgreSql(list.value(i));
    }
    else if(m_driverName == "QSQLITE")
    {
        fillTableTypesMySql();
        for(int i = 0; i < list.size(); i++)
            script += createSqlScriptForTableSqlite(list.value(i));
        for(int i = 0; i < list.size(); i++)
            script += createRelationForTableSqlite(list.value(i));

    }
    return script;
}

bool SqlScriptBuilder::setDriverName(const QString &driverName)
{
    QStringList supportDBMS;
    supportDBMS << "QMYSQL" << "QPSQL" << "QSQLITE";
    if(supportDBMS.contains(driverName))
    {
        m_driverName = driverName;
        return true;
    }
    else
        return false;
}

QString SqlScriptBuilder::createSqlScriptForTableMySql(const Model &model) const
{
    QString script = QString("CREATE TABLE %1(id BIGINT AUTO_INCREMENT, ")
            .arg(model.name());
    Logger::generateScheme(model.name());
    for(int i = 0; i < model.properties().size(); i++)
    {
        script += QString("\n         %1 %2, ")
                .arg(model.properties().value(i).getName())
                .arg(m_tableTypes.value(model.properties().value(i).getType()));
    }
    script += "\n         PRIMARY KEY(id));\n";
    return script;
}

QString SqlScriptBuilder::createRelationForTableMySql(const Model &model) const
{
    QString script;
    QString patternHasMany = QString("ALTER TABLE %1 ADD %2_id BIGINT AFTER id, \n      ADD FOREIGN KEY(%2_id) REFERENCES %2(id);");
    QString patternHasOne = QString("ALTER TABLE %1 ADD %2_id BIGINT AFTER id, \n      ADD FOREIGN KEY(%2_id) REFERENCES %2(id), "
                                    "\n      ADD UNIQUE(%2_id);");
    for(int i = 0; i < model.hasOne().size(); i++)
    {
        script += patternHasOne
                .arg(model.hasOne().value(i))
                .arg(model.name());
        script += '\n';
    }
    for(int i = 0; i < model.hasMany().size(); i++)
    {
        script += patternHasMany
                .arg(model.hasMany().value(i))
                .arg(model.name());
        script += '\n';
    }
    return script;
}

QString SqlScriptBuilder::createSqlScriptForTablePostgreSql(const Model &model) const
{
    QString script = QString("CREATE TABLE %1(id SERIAL, ")
            .arg(model.name());
    Logger::generateScheme(model.name());
    for(int i = 0; i < model.properties().size(); i++)
    {
        script += QString("\n         %1 %2, ")
                .arg(model.properties().value(i).getName())
                .arg(m_tableTypes.value(model.properties().value(i).getType()));
    }
    script += "\n         PRIMARY KEY(id));\n";
    return script;
}

QString SqlScriptBuilder::createRelationForTablePostgreSql(const Model &model) const
{
    QString script;
    QString patternHasMany = QString("ALTER TABLE %1 ADD %2_id BIGINT, \n      ADD FOREIGN KEY(%2_id) REFERENCES %2(id);\n");
    QString patternHasOne = QString("ALTER TABLE %1 ADD %2_id BIGINT, \n      ADD FOREIGN KEY(%2_id) REFERENCES %2(id), "
                                    "\n      ADD UNIQUE(%2_id);\n");
    for(int i = 0; i < model.hasOne().size(); i++)
        script += patternHasOne
                .arg(model.hasOne().value(i))
                .arg(model.name());
    for(int i = 0; i < model.hasMany().size(); i++)
        script += patternHasMany
                .arg(model.hasMany().value(i))
                .arg(model.name());
    return script;
}

QString SqlScriptBuilder::createSqlScriptForTableSqlite(const Model &model) const
{
    QString script = QString("CREATE TABLE %1(id INTEGER PRIMARY KEY, ")
            .arg(model.name());
    Logger::generateScheme(model.name());
    for(int i = 0; i < model.properties().size(); i++)
    {
        script += QString("\n         %1 %2, ")
                .arg(model.properties().value(i).getName())
                .arg(m_tableTypes.value(model.properties().value(i).getType()));
    }
    script.resize(script.size() - 2);
    script += ");\n";
    return script;
}

QString SqlScriptBuilder::createRelationForTableSqlite(const Model &model) const
{
    QString script;
    QString pattern = QString("ALTER TABLE %1 ADD %2_id INTEGER;");
    for(int i = 0; i < model.hasOne().size(); i++)
    {
        script += pattern
                .arg(model.hasOne().value(i))
                .arg(model.name());
        script += '\n';
    }
    for(int i = 0; i < model.hasMany().size(); i++)
    {
        script += pattern
                .arg(model.hasMany().value(i))
                .arg(model.name());
        script += '\n';
    }
    return script;
}

void SqlScriptBuilder::fillTableTypesMySql()
{
    m_tableTypes.insert("bool", "BOOL");
    m_tableTypes.insert("int", "INT");
    m_tableTypes.insert("uint", "INT UNSIGNED");
    m_tableTypes.insert("qlonglong", "BIGINT");
    m_tableTypes.insert("qulonglong", "BIGINT UNSIGNED");
    m_tableTypes.insert("double", "DOUBLE");
    m_tableTypes.insert("QByteArray", "BLOB");
    m_tableTypes.insert("QChar", "CHAR(1)");
    m_tableTypes.insert("QDate", "DATE");
    m_tableTypes.insert("QTime", "TIME");
    m_tableTypes.insert("QDateTime", "DATETIME");
    m_tableTypes.insert("QString", "TEXT");
}

void SqlScriptBuilder::fillTableTypesPostgreSql()
{
    m_tableTypes.insert("bool", "BOOL");
    m_tableTypes.insert("int", "INT");
    m_tableTypes.insert("uint", "INT");
    m_tableTypes.insert("qlonglong", "BIGINT");
    m_tableTypes.insert("qulonglong", "BIGINT");
    m_tableTypes.insert("double", "DOUBLE PRECISION");
    m_tableTypes.insert("QByteArray", "BYTEA");
    m_tableTypes.insert("QChar", "CHAR(1)");
    m_tableTypes.insert("QDate", "DATE");
    m_tableTypes.insert("QTime", "TIME");
    m_tableTypes.insert("QDateTime", "TIMESTAMP");
    m_tableTypes.insert("QString", "TEXT");
}
