#include "parser.h"
#include <QDebug>

Parser::Parser()
{
}

void Parser::process(QStringList files)
{
    QString currentPath;
    foreach (currentPath, files)
    {
        QFile file(currentPath);
        file.open(QIODevice::ReadOnly);
        QTextStream stream(&file);
        while(!stream.atEnd())
            m_currentFile += removeTrash(stream.readLine());
        m_currentFile = simplified(m_currentFile);

    }
}

QString Parser::getCurrentFile()
{
    return m_currentFile;
}

QList<QString> Parser::cutModelInfo(const QString &str)
{
    QList<QString> list;
    QRegularExpression e("ORMObject<(\\w+)>");
    QRegularExpressionMatchIterator it = e.globalMatch(str);
    while(it.hasNext())
    {
        QRegularExpressionMatch m = it.next();
        if(it.hasNext())
            list.append(str.mid(m.capturedRef(0).position(), it.peekNext().capturedRef(0).position() - m.capturedRef(0).position()));
        else
            list.append(str.mid(m.capturedRef(0).position(), str.length() - m.capturedRef(0).position()));
    }
    return list;
}

QString Parser::removeTrash(QString str)
{
    str.remove(QRegularExpression("\".*[^\\\\]\""));
    if(str.indexOf("//") < str.indexOf("*/")) //  abc//def
        str.remove(str.indexOf("//"), str.indexOf("*/") - str.indexOf("//"));
    str.remove(QRegularExpression("//.*"));
    return str;
}

QString &Parser::simplified(QString &str)
{
    str = str.simplified();
    str.remove(QRegularExpression("/\\*.*\\*/"));
    return str;
}
