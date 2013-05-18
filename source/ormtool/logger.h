#ifndef LOGGER_H
#define LOGGER_H

#include <QTextStream>

class Logger
{
public:
    Logger();
    enum Relation
    {
        HasOne,
        HasMany
    };
    static void warning(QString msg);
    static void foundModel(QString modelName);
    static void foundProperty(QString modelName, QString type, QString name);
    static void foundRelation(QString modelName, Relation rel, QString name);
    static void generateScheme(QString name);
    static void done();
    static QTextStream m_stream;
};

#endif // LOGGER_H
