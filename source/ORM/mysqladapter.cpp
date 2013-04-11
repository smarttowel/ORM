#include "mysqladapter.h"

MySqlAdapter::MySqlAdapter()
{
    fillTableTypes();
}

bool MySqlAdapter::createDatabase(const QString name)
{
    m_lastQuery = QString("CREATE DATABASE %1;")
            .arg(name);
    bool result = m_query.exec(m_lastQuery);
    if(result)
        initDB(name);
    return result;
}

bool MySqlAdapter::createTable(const QString tableName, const QHash<QString, QString> &info)
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

bool MySqlAdapter::dropTable(const QString tableName)
{
    m_lastQuery = QString("DROP TABLE %1;")
            .arg(tableName);
    return m_query.exec(m_lastQuery);
}

bool MySqlAdapter::dropDatabase(const QString name)
{
    m_lastQuery = QString("DROP DATABASE %1")
            .arg(name);
    return m_query.exec(m_lastQuery);
}

int MySqlAdapter::addRecord(const QString tableName, const QHash<QString, QVariant> &info)
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

bool MySqlAdapter::updateRecord(const QString tableName, const qlonglong id, const QHash<QString, QVariant> &info)
{
    m_lastQuery = QString("UPDATE %1 SET ")
            .arg(tableName);
    QString fieldName;
    foreach(fieldName, info.keys())
        m_lastQuery += QString("%1 = '%2', ")
                .arg(fieldName)
                .arg(info.value(fieldName).toString());
    m_lastQuery.resize(m_lastQuery.size() - 2);
    m_lastQuery += QString(" WHERE id = %1;")
            .arg(id);
    return m_query.exec(m_lastQuery);
}

QList<QSqlRecord> MySqlAdapter::find(const QString tableName, const QString findString)
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

QList<QSqlRecord> MySqlAdapter::findAll(const QString tableName)
{
    QList<QSqlRecord> result;
    m_lastQuery = QString("SELECT * FROM %1;")
            .arg(tableName);
    if(m_query.exec(m_lastQuery))
        while(m_query.next())
            result.append(m_query.record());
    return result;
}

QSqlRecord MySqlAdapter::first(const QString tableName)
{
    m_lastQuery = QString("SELECT * FROM %1 ORDER BY id ASC LIMIT 1;")
            .arg(tableName);
    m_query.exec(m_lastQuery);
    m_query.next();
    return m_query.record();
}

QSqlRecord MySqlAdapter::last(const QString tableName)
{
    m_lastQuery = QString("SELECT * FROM %1 ORDER BY id DESC LIMIT 1;")
            .arg(tableName);
    m_query.exec(m_lastQuery);
    m_query.next();
    return m_query.record();
}

bool MySqlAdapter::remove(const QString tableName, const QString whereString)
{
    m_lastQuery = QString("DELETE FROM %1 WHERE %2;")
            .arg(tableName)
            .arg(whereString);
    return m_query.exec(m_lastQuery);
}

bool MySqlAdapter::removeAll(const QString tableName)
{
    m_lastQuery = QString("DELETE FROM %1;")
            .arg(tableName);
    return m_query.exec(m_lastQuery);
}

int MySqlAdapter::count(const QString tableName, const QString arg)
{
    m_lastQuery = QString("SELECT COUNT(%1) FROM %2;")
            .arg(arg)
            .arg(tableName);
    if(m_query.exec(m_lastQuery))
    {
        m_query.next();
        return m_query.value(0).toInt();
    }
    else
        return -1;
}

int MySqlAdapter::countBy(const QString tableName, const QString whereString)
{
    m_lastQuery = QString("SELECT COUNT(*) FROM %1 WHERE %2;")
            .arg(tableName)
            .arg(whereString);
    if(m_query.exec(m_lastQuery))
    {
        m_query.next();
        return m_query.value(0).toInt();
    }
    else
        return -1;
}

double MySqlAdapter::average(const QString tableName, const QString fieldName)
{
    m_lastQuery = QString("SELECT AVG(%1) FROM %2;")
            .arg(fieldName)
            .arg(tableName);
    m_query.exec(m_lastQuery);
    m_query.next();
    return m_query.value(0).toDouble();
}

double MySqlAdapter::average(const QString tableName, const QString fieldName, const QString whereString)
{
    m_lastQuery = QString("SELECT AVG(%1) FROM %2 WHERE %3;")
            .arg(fieldName)
            .arg(tableName)
            .arg(whereString);
    m_query.exec(m_lastQuery);
    m_query.next();
    return m_query.value(0).toDouble();
}

double MySqlAdapter::maximum(const QString tableName, const QString fieldName)
{
    m_lastQuery = QString("SELECT MAX(%1) FROM %2;")
            .arg(fieldName)
            .arg(tableName);
    m_query.exec(m_lastQuery);
    m_query.next();
    return m_query.value(0).toDouble();
}

double MySqlAdapter::maximum(const QString tableName, const QString fieldName, const QString whereString)
{
    m_lastQuery = QString("SELECT MAX(%1) FROM %2 WHERE %3;")
            .arg(fieldName)
            .arg(tableName)
            .arg(whereString);
    m_query.exec(m_lastQuery);
    m_query.next();
    return m_query.value(0).toDouble();
}

double MySqlAdapter::minimum(const QString tableName, const QString fieldName)
{
    m_lastQuery = QString("SELECT MIN(%1) FROM %2;")
            .arg(fieldName)
            .arg(tableName);
    m_query.exec(m_lastQuery);
    m_query.next();
    return m_query.value(0).toDouble();
}

double MySqlAdapter::minimum(const QString tableName, const QString fieldName, const QString whereString)
{
    m_lastQuery = QString("SELECT MIN(%1) FROM %2 WHERE %3;")
            .arg(fieldName)
            .arg(tableName)
            .arg(whereString);
    m_query.exec(m_lastQuery);
    m_query.next();
    return m_query.value(0).toDouble();
}

double MySqlAdapter::sum(const QString tableName, const QString fieldName)
{
    m_lastQuery = QString("SELECT SUM(%1) FROM %2;")
            .arg(fieldName)
            .arg(tableName);
    m_query.exec(m_lastQuery);
    m_query.next();
    return m_query.value(0).toDouble();
}

double MySqlAdapter::sum(const QString tableName, const QString fieldName, const QString whereString)
{
    m_lastQuery = QString("SELECT SUM(%1) FROM %2 WHERE %3;")
            .arg(fieldName)
            .arg(tableName)
            .arg(whereString);
    m_query.exec(m_lastQuery);
    m_query.next();
    return m_query.value(0).toDouble();
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
