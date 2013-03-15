#ifndef ORMOBJECT_H
#define ORMOBJECT_H

#include <QObject>

class ORMObject : public QObject
{
    Q_OBJECT
public:
    explicit ORMObject(QObject *parent = 0);
    
signals:
    
public slots:
    
};

#endif // ORMOBJECT_H
