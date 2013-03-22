#ifndef MYSQLADAPTER_H
#define MYSQLADAPTER_H

#include <QVariant>
#include "ormabstractadapter.h"

class MySqlAdapter : public ORMAbstractAdapter
{
public:
    MySqlAdapter();
    bool createDatabase(QString name);
    bool createTable(QHash<QString, QString> fieldsInfo);

protected:
    void fillTableTypes();

private:
    void initDB(QString name);
};

#endif // MYSQLADAPTER_H
