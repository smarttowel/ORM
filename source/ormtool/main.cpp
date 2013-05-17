#include <QCoreApplication>
#include <QStringList>
#include <QTextStream>
#include <QFile>
#include "parser.h"
#include "sqlscriptbuilder.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QStringList files;
    QTextStream out(stdout);
    for(int i = 1; i < argc; i++)
        files.append(argv[i]);
    if(files.isEmpty())
        out << "No input files!";
    Parser parser;
    SqlScriptBuilder builder;
    QFile outputFile("CreateTables.sql");
    outputFile.open(QIODevice::WriteOnly);
    QTextStream stream(&outputFile);
    stream << builder.process(parser.process(files));
    outputFile.close();
    out << "Done!" << endl;
//    return a.exec();
}
