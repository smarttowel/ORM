#ifndef ORMABSTRACTADAPTER_H
#define ORMABSTRACTADAPTER_H

#include <QObject>

class ORMAbstractAdapter : public QObject
{
    Q_OBJECT
public:
    explicit ORMAbstractAdapter(QObject *parent = 0);
    
signals:
    
public slots:
    
};

#endif // ORMABSTRACTADAPTER_H
