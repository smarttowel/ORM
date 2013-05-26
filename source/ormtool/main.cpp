#include <QCoreApplication>
#include <QStringList>
#include <QTextStream>
#include <QFile>
#include "parser.h"
#include "sqlscriptbuilder.h"
#include "logger.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QStringList files;
    for(int i = 2; i < argc; i++)
        files.append(argv[i]);
    QString key = argv[1];
    if(key == "help" || key == "--help" || key == "-help" || key.isEmpty())
    {
        Logger::showHelp();
        return 0;
    }
    Parser parser;
    SqlScriptBuilder builder;
    if(!builder.setDriverName(key))
    {
        Logger::warning("ormtool doesn't support this DBMS!");
        return 0;
    }
    if(files.isEmpty())
    {
        Logger::warning("No input files!");
        return 0;
    }
    QList<Model> list = parser.process(files);
    QFile outputFile("CreateTables.sql");
    outputFile.open(QIODevice::WriteOnly);
    QTextStream stream(&outputFile);
    stream << builder.process(list);
    outputFile.close();
    Logger::done();
}
