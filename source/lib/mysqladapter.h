#ifndef MYSQLADAPTER_H
#define MYSQLADAPTER_H

#include <QVariant>
#include <QSqlRecord>
#include <QStringList>
#include <QSqlField>
#include "sqladapter.h"

class MySqlAdapter : public SqlAdapter
{
public:
    MySqlAdapter();
    bool createDatabase(const QString &name);

private:
    void initDB(const QString &name);
};

#endif // MYSQLADAPTER_H
