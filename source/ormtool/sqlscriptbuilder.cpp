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

QString SqlScriptBuilder::process(QList<Model> list)
{

}

QString SqlScriptBuilder::createSqlScriptForTable(Model model)
{
    QString script = QString("CREATE TABLE %1(id BIGINT AUTO_INCREMENT, ")
            .arg(model.name());
    for(int i = 0; i < model.properties().size(); i++)
    {
        script += QString("%1 %2, ")
                .arg(model.properties().value(i).getName())
                .arg(m_tableTypes.value(model.properties().value(i).getType()));
    }
    script += "PRIMARY KEY(id));";
    return script;
}
