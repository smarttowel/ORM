#ifndef ORMOBJECT_H
#define ORMOBJECT_H

#define ORM_PROPERTY(type, name) \
    Q_PROPERTY(type name READ get##name WRITE set##name) \
    public: \
    type name; \
    type get##name(){ return name; } \
    void set##name(type input_##name) { name = input_##name; } \

#include <QObject>
#include <QMetaProperty>
#include "ormdatabase.h"

//TODO: make toList<> method
//TODO: make toModel<> method

class ORMObject : public QObject
{
    Q_OBJECT
public:
    explicit ORMObject(QObject *parent = 0);
    bool createTable();
};

#endif // ORMOBJECT_H
