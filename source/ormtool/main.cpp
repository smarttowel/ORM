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
    for(int i = 1; i < argc; i++)
        files.append(argv[i]);
    if(files.isEmpty())
        Logger::warning("No input files!");
    Parser parser;
    SqlScriptBuilder builder;
    QList<Model> list = parser.process(files);
    QFile outputFile("CreateTables.sql");
    outputFile.open(QIODevice::WriteOnly);
    QTextStream stream(&outputFile);
    stream << builder.process(list);
    outputFile.close();
    Logger::done();
}
