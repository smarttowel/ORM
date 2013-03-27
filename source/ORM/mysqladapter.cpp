#include "mysqladapter.h"

MySqlAdapter::MySqlAdapter()
{
    fillTableTypes();
}

bool MySqlAdapter::createDatabase(QString name)
{
    m_lastQuery = QString("CREATE DATABASE %1;")
            .arg(name);
    bool result = m_query.exec(m_lastQuery);
    if(result)
        initDB(name);
    return result;
}

bool MySqlAdapter::createTable(QString tableName, QHash<QString, QString> info)
{
    QString name;
    m_lastQuery = QString("CREATE TABLE %1(id INT AUTO_INCREMENT, ")
            .arg(tableName);
    foreach(name, info.keys())
        m_lastQuery += QString("%1 %2, ")
                .arg(name)
                .arg(m_tableTypes.value(info.value(name)));
    m_lastQuery += "PRIMARY KEY (id));";
    return m_query.exec(m_lastQuery);
}

bool MySqlAdapter::dropTable(QString tableName)
{
    m_lastQuery = QString("DROP TABLE %1;")
            .arg(tableName);
    return m_query.exec(m_lastQuery);
}

bool MySqlAdapter::dropDatabase(QString name)
{
    m_lastQuery = QString("DROP DATABASE %1")
            .arg(name);
    return m_query.exec(m_lastQuery);
}

int MySqlAdapter::addRecord(QString tableName, QHash<QString, QVariant> info)
{
    QString key;
    m_lastQuery = QString("INSERT INTO %1(")
            .arg(tableName);
    foreach(key, info.keys())
        m_lastQuery += key + ", ";
    m_lastQuery.resize(m_lastQuery.size() - 2);
    m_lastQuery += ") VALUES(";
    foreach(key, info.keys())
        m_lastQuery += "'" + info.value(key).toString() + "', ";
    m_lastQuery.resize(m_lastQuery.size() - 2);
    m_lastQuery += ");";
    if(m_query.exec(m_lastQuery))
        return m_query.lastInsertId().toInt();
    else
        return -1;
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
