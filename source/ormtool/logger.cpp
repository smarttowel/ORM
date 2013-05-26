#include "logger.h"

QTextStream Logger::m_stream(stdout);

Logger::Logger()
{
}

void Logger::warning(QString msg)
{
    m_stream << "WARNING: " << msg << endl;
}

void Logger::foundModel(QString modelName)
{
    m_stream << "Found model: " << modelName << endl;
}

void Logger::foundProperty(QString modelName, QString type, QString name)
{
    m_stream << modelName << ": found property (" << type << " : " << name << ')' << endl;
}

void Logger::foundRelation(QString modelName, Logger::Relation rel, QString name)
{
    if(rel == HasOne)
        m_stream << modelName << ": found \"has one\" relation: " << name << endl;
    else
        m_stream << modelName << ": found \"has many\" relation: " << name << endl;
}

void Logger::generateScheme(QString name)
{
    m_stream << "Generate scheme for " << name << "..." << endl;
}

void Logger::showHelp()
{
    m_stream << "Usage:" << endl
             << "     ormtool [argument] [file1 file2 ... fileN]" << endl
             << "argument:" << endl
             << "     QMYSQL - generate script for MySQL;" << endl
             << "     QPSQL - generate script for PostgreSQL;" << endl
             << "     QSQLITE - generate script for SQLite;" << endl
             << "     help - show this help." << endl;
}

void Logger::done()
{
    m_stream << "Done!" << endl;
}
