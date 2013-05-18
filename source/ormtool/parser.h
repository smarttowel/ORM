#ifndef PARSER_H
#define PARSER_H

#include <QFile>
#include <QTextStream>
#include <QRegularExpression>
#include <QStringRef>
#include "model.h"
#include "property.h"
#include "logger.h"


class Parser
{
public:
    Parser();
    QList<Model> process(QStringList files);

private:
    QRegularExpression m_regExpObject;
    QRegularExpression m_regExpProperty;
    QRegularExpression m_regExpHasOne;
    QRegularExpression m_regExpHasMany;
    QRegularExpression m_regExpLiteral;
    QRegularExpression m_regExpLineComment;
    QRegularExpression m_regExpMultilineComment;
    QList<Model> m_models;
    QList<QStringRef> m_modelsRef;
    QString m_currentFile;
    QString removeTrash(QString str);
    QString& simplified(QString &str) const;
    QString getCurrentFile() const;
    QList<QString> cutModelInfo(const QString &str) const;
    Model getModelFromString(const QString &str) const;
};

#endif // PARSER_H
