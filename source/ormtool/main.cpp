#include <QCoreApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    qDebug() << argc;
    for(int i = 0; i < argc; i++)
        qDebug() << argv[i];

//    return a.exec();
}
