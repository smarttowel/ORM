#ifndef MYSQLADAPTER_H
#define MYSQLADAPTER_H

#include <QVariant>
#include "ormabstractadapter.h"

class MySqlAdapter : public ORMAbstractAdapter
{
public:
    MySqlAdapter();
    bool createDatabase(QString name);
};

#endif // MYSQLADAPTER_H
