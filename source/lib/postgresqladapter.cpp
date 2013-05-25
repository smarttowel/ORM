#include "postgresqladapter.h"

PostgreSQLAdapter::PostgreSQLAdapter()
{
    fillTableTypes();
}

bool PostgreSQLAdapter::createTable(const QString &tableName, const QHash<QString, QString> &info)
{
    QString name;
    m_lastQuery = QString("CREATE TABLE %1(id BIGSERIAL, ")
            .arg(tableName);
    foreach(name, info.keys())
        m_lastQuery += QString("%1 %2, ")
                .arg(name)
                .arg(m_tableTypes.value(info.value(name)));
    m_lastQuery += "PRIMARY KEY (id));";
    return m_query.exec(m_lastQuery);
}

bool PostgreSQLAdapter::createTableRelations(const QString &parent, ORMAbstractAdapter::Relation rel, const QString &child)
{
    if(rel == HasMany)
        m_lastQuery = QString("ALTER TABLE %1 ADD %2_id BIGINT, ADD FOREIGN KEY(%2_id) REFERENCES %2(id);")
                .arg(child)
                .arg(parent);
    else if(rel == HasOne)
        m_lastQuery = QString("ALTER TABLE %1 ADD %2_id BIGINT, ADD FOREIGN KEY(%2_id) REFERENCES %2(id),"
                              "ADD UNIQUE(%2_id);")
                .arg(child)
                .arg(parent);
    return m_query.exec(m_lastQuery);
}

int PostgreSQLAdapter::addRecord(const QString &tableName, const QHash<QString, QVariant> &info)
{
    QString key;
    if(info.isEmpty())
        m_lastQuery = QString("INSERT INTO %1 DEFAULT VALUES RETURNING id;")
                .arg(tableName);
    else
    {
        m_lastQuery = QString("INSERT INTO %1(")
                .arg(tableName);
        foreach(key, info.keys())
            m_lastQuery += key + ", ";
        if(!info.isEmpty())
            m_lastQuery.resize(m_lastQuery.size() - 2);
        m_lastQuery += ") VALUES(";
        foreach(key, info.keys())
            m_lastQuery += "'" + info.value(key).toString() + "', ";
        if(!info.isEmpty())
            m_lastQuery.resize(m_lastQuery.size() - 2);
        m_lastQuery += ") RETURNING id;";
    }
    if(m_query.exec(m_lastQuery))
    {
        m_query.next();
        return m_query.record().value(0).toInt();
    }
    else
        return -1;
}

void PostgreSQLAdapter::fillTableTypes()
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
