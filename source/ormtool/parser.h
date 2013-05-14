#ifndef PARSER_H
#define PARSER_H

#include <QFile>
#include <QTextStream>
#include <QRegularExpression>
#include <QStringRef>
#include "model.h"

class Parser
{
public:
    Parser();
    void process(QStringList files);
    QString removeTrash(QString str);
    void simplified();
    QString getCurrentFile();

private:
    QList<Model> m_models;
    QList<QStringRef> m_modelsRef;
    QString m_currentFile;
    QRegularExpression m_modelNamePattern;
};

#endif // PARSER_H
