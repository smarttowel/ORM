#include "sqliteadapter.h"
#include <QDebug>

SqliteAdapter::SqliteAdapter()
{
}

bool SqliteAdapter::createTable(const QString &tableName, const QHash<QString, QString> &info)
{
    QString name;
    m_lastQuery = QString("CREATE TABLE %1(id BIGINT, ")
            .arg(tableName);
    foreach(name, info.keys())
        m_lastQuery += QString("%1 %2, ")
                .arg(name)
                .arg(m_tableTypes.value(info.value(name)));
    m_lastQuery += "PRIMARY KEY (id));";
    m_query.clear();
    return m_query.exec(m_lastQuery);
}

bool SqliteAdapter::createTableRelations(const QString &parent, ORMAbstractAdapter::Relation rel, const QString &child)
{
    m_lastQuery = QString("ALTER TABLE %1 ADD %2_id BIGINT;")
            .arg(child)
            .arg(parent);
    return m_query.exec(m_lastQuery);
}
