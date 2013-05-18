#include "sqlscriptbuilder.h"

SqlScriptBuilder::SqlScriptBuilder()
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

QString SqlScriptBuilder::process(const QList<Model> &list)
{
    QString script;
    for(int i = 0; i < list.size(); i++)
        script += createSqlScriptForTable(list.value(i));
    for(int i = 0; i < list.size(); i++)
        script += createRelationsForTable(list.value(i));
    return script;
}

QString SqlScriptBuilder::createSqlScriptForTable(const Model &model) const
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

QString SqlScriptBuilder::createRelationsForTable(const Model &model) const
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
