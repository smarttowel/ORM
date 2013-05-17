#include "parser.h"

Parser::Parser()
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
        file.open(QIODevice::ReadOnly);
        QTextStream stream(&file);
        while(!stream.atEnd())
            m_currentFile += removeTrash(stream.readLine());
        m_currentFile = simplified(m_currentFile);
        list = cutModelInfo(m_currentFile);
        for(int i = 0; i < list.size(); i++)
            returnList.append(getModelFromString(list.value(i)));
        m_currentFile.clear();
        file.close();
    }
    return returnList;
}

QString Parser::getCurrentFile()
{
    return m_currentFile;
}

QList<QString> Parser::cutModelInfo(const QString &str)
{
    QList<QString> list;
    QRegularExpression e("ORMObject<\\w+>");
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

Model Parser::getModelFromString(const QString &str)
{
    QTextStream stream(stdout);
    QRegularExpression object("ORMObject<(\\w+)>");
    QRegularExpression property("ORM_PROPERTY\\((\\w+), ?(\\w+)\\)");
    QRegularExpression hasOne("ORM_HAS_ONE\\((\\w+)\\)");
    QRegularExpression hasMany("ORM_HAS_MANY\\((\\w+)\\)");
    QRegularExpressionMatch m = object.match(str);
    QString name = m.captured(1);
    stream << "Found model: " << name << endl;
    Model model(name);
    QRegularExpressionMatchIterator it = property.globalMatch(str);
    while(it.hasNext())
    {
        m = it.next();
        stream << name << ": found property (" << m.captured(1) << ':' << m.captured(2) << ")" << endl;
        model.addProperty(Property(m.captured(1), m.captured(2)));
    }
    it = hasOne.globalMatch(str);
    while(it.hasNext())
    {
        m = it.next();
        stream << name << ": found \"has one\" relation: " << m.captured(1) << endl;
        model.addHasOne(m.captured(1));
    }
    it = hasMany.globalMatch(str);
    while(it.hasNext())
    {
        m = it.next();
        stream << name << ": found \"has many\" relation: " << m.captured(1) << endl;
        model.addHasMany(m.captured(1));
    }
    return model;
}

QString Parser::removeTrash(QString str)
{
    str.remove(QRegularExpression("\".*[^\\\\]\""));
    if(str.indexOf("//") < str.indexOf("*/"))
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
