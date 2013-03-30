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

bool MySqlAdapter::createTable(QString tableName, const QHash<QString, QString> &info)
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

int MySqlAdapter::addRecord(QString tableName, const QHash<QString, QVariant> &info)
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

QList<QSqlRecord> MySqlAdapter::find(QString tableName, QString findString)
{
    QList<QSqlRecord> result;
    m_lastQuery = QString("SELECT * FROM %1 WHERE %2;")
            .arg(tableName)
            .arg(findString);
    if(m_query.exec(m_lastQuery))
        while(m_query.next())
            result.append(m_query.record());
    return result;
}

QSqlRecord MySqlAdapter::first(QString tableName)
{
    m_lastQuery = QString("SELECT * FROM %1 ORDER BY id ASC LIMIT 1;")
            .arg(tableName);
    m_query.exec(m_lastQuery);
    m_query.next();
    return m_query.record();
}

QSqlRecord MySqlAdapter::last(QString tableName)
{
    m_lastQuery = QString("SELECT * FROM %1 ORDER BY id DESC LIMIT 1;")
            .arg(tableName);
    m_query.exec(m_lastQuery);
    m_query.next();
    return m_query.record();
}

QSqlRecord MySqlAdapter::findBy(QString tableName, QString fieldName, QVariant value)
{
    m_lastQuery = QString("SELECT * FROM %1 WHERE %2 = '%3'")
            .arg(tableName)
            .arg(fieldName)
            .arg(value.toString());
    m_query.exec(m_lastQuery);
    m_query.next();
    return m_query.record();
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
