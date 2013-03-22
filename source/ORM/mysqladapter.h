#ifndef MYSQLADAPTER_H
#define MYSQLADAPTER_H

#include <QVariant>
#include "ormabstractadapter.h"

class MySqlAdapter : public ORMAbstractAdapter
{
public:
    MySqlAdapter();
    bool createDatabase(QString name);

protected:
    void fillTableTypes();

private:
    void initDB(QString name);
};

#endif // MYSQLADAPTER_H
