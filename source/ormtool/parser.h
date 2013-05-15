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
    QString& simplified(QString &str);
    QString getCurrentFile();
    QList<QString> cutModelInfo(const QString &str);

private:
    QList<Model> m_models;
    QList<QStringRef> m_modelsRef;
    QString m_currentFile;
};

#endif // PARSER_H
