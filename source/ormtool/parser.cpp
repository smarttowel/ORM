#include "parser.h"

Parser::Parser() : m_regExpObject("ORMObject<(\\w+)>"),
    m_regExpProperty("ORM_PROPERTY\\((\\w+), ?(\\w+)\\)"),
    m_regExpHasOne("ORM_HAS_ONE\\((\\w+)\\)"),
    m_regExpHasMany("ORM_HAS_MANY\\((\\w+)\\)"),
    m_regExpLiteral("\".*[^\\\\]\""),
    m_regExpLineComment("//.*"),
    m_regExpMultilineComment("/\\*.*\\*/")

{
}

QList<Model> Parser::process(QStringList files)
{
    QString currentPath;
    QList<QString> list;
    QList<Model> returnList;
    foreach (currentPath, files)
    {
        QFile file(currentPath);
        if(!file.open(QIODevice::ReadOnly))
        {
            Logger::warning(file.errorString());
            continue;
        }
        QTextStream stream(&file);
        while(!stream.atEnd())
            m_currentFile += removeTrash(stream.readLine());
        m_currentFile = simplified(m_currentFile);
        list = cutModelInfo(m_currentFile);
        if(list.isEmpty())
             Logger::warning("In file " + file.fileName() + " models not found!");
        for(int i = 0; i < list.size(); i++)
            returnList.append(getModelFromString(list.value(i)));
        m_currentFile.clear();
        file.close();
    }
    return returnList;
}

QString Parser::getCurrentFile() const
{
    return m_currentFile;
}

QList<QString> Parser::cutModelInfo(const QString &str) const
{
    QList<QString> list;
    QRegularExpressionMatchIterator it = m_regExpObject.globalMatch(str);
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

Model Parser::getModelFromString(const QString &str) const
{
    QRegularExpressionMatch m = m_regExpObject.match(str);
    QString name = m.captured(1);
    Logger::foundModel(name);
    Model model(name);
    QRegularExpressionMatchIterator it = m_regExpProperty.globalMatch(str);
    while(it.hasNext())
    {
        m = it.next();
        Logger::foundProperty(name, m.captured(1), m.captured(2));
        model.addProperty(Property(m.captured(1), m.captured(2)));
    }
    it = m_regExpHasOne.globalMatch(str);
    while(it.hasNext())
    {
        m = it.next();
        Logger::foundRelation(name, Logger::HasOne, m.captured(1));
        model.addHasOne(m.captured(1));
    }
    it = m_regExpHasMany.globalMatch(str);
    while(it.hasNext())
    {
        m = it.next();
        Logger::foundRelation(name, Logger::HasMany, m.captured(1));
        model.addHasMany(m.captured(1));
    }
    return model;
}

QString Parser::removeTrash(QString str)
{
    str.remove(m_regExpLiteral);
    if(str.indexOf("//") < str.indexOf("*/"))
        str.remove(str.indexOf("//"), str.indexOf("*/") - str.indexOf("//"));
    str.remove(m_regExpLineComment);
    return str;
}

QString &Parser::simplified(QString &str) const
{
    str = str.simplified();
    str.remove(m_regExpMultilineComment);
    return str;
}
