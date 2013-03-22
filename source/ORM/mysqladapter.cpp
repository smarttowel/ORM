#include "mysqladapter.h"

MySqlAdapter::MySqlAdapter()
{
}

bool MySqlAdapter::createDatabase(QString name)
{
    m_lastQuery = QString("CREATE DATABASE %1;")
            .arg(name);
    bool result = m_query.exec(m_lastQuery);
    initDB(name);
    return result;
}

void MySqlAdapter::initDB(QString name)
{
    m_lastQuery = QString("USE %1;")
            .arg(name);
    m_query.exec(m_lastQuery);
}

void MySqlAdapter::fillTableTypes()
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
