#include "sqliteadapter.h"

SqliteAdapter::SqliteAdapter()
{
    fillTableTypes();
}

bool SqliteAdapter::createTable(const QString &tableName, const QHash<QString, QString> &info)
{
    QString name;
    m_lastQuery = QString("CREATE TABLE %1(id INTEGER PRIMARY KEY, ")
            .arg(tableName);
    foreach(name, info.keys())
        m_lastQuery += QString("%1 %2, ")
                .arg(name)
                .arg(m_tableTypes.value(info.value(name)));
    m_lastQuery.resize(m_lastQuery.size() - 2);
    m_lastQuery.append(");");
    m_query.clear();
    return m_query.exec(m_lastQuery);
}

int SqliteAdapter::addRecord(const QString &tableName, const QHash<QString, QVariant> &info)
{
    QString key;
    if(info.isEmpty())
        m_lastQuery = QString("INSERT INTO %1 DEFAULT VALUES;")
                .arg(tableName);
    else
    {
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
    }
    if(m_query.exec(m_lastQuery))
        return m_query.lastInsertId().toInt();
    else
        return -1;
}
