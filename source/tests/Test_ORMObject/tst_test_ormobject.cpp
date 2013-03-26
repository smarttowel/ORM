#include <QObject>
#include <QString>
#include <QtTest>
#include <QSqlRecord>
#include <QSqlField>
#include "ormdatabase.cpp"
#include "mysqladapter.cpp"
#include "ormabstractadapter.cpp"
#include "ormobject.cpp"

class MyModel : public ORMObject
{
    Q_OBJECT
    ORM_PROPERTY(bool, nameBool)
    ORM_PROPERTY(int, nameInt)
    ORM_PROPERTY(uint, nameUint)
    ORM_PROPERTY(qlonglong, nameLonglong)
    ORM_PROPERTY(qulonglong, nameUlonglong)
    ORM_PROPERTY(double, namedouble)
    ORM_PROPERTY(QByteArray, nameBlob)
    ORM_PROPERTY(QChar, nameChar)
    ORM_PROPERTY(QDate, nameDate)
    ORM_PROPERTY(QTime, nameTime)
    ORM_PROPERTY(QDateTime, nameDatetime)
    ORM_PROPERTY(QString, nameString)
public:
    MyModel() {}
};

class Test_ORMObject : public QObject
{
    Q_OBJECT
    
public:
    Test_ORMObject();
    ~Test_ORMObject();
    
private:
    ORMDatabase db;

private Q_SLOTS:
    void test_CreateTable();
    void test_save();
};

Test_ORMObject::Test_ORMObject()
{
    db = ORMDatabase::addORMDatabase("QMYSQL");
    db.setUserName("root");
    db.setHostName("localhost");
    qDebug() << db.createDatabase("Test_ORMDatabase");
}

Test_ORMObject::~Test_ORMObject()
{
    db.exec("DROP DATABASE Test_ORMDatabase;");
}

void Test_ORMObject::test_CreateTable()
{
    MyModel model;
    QCOMPARE(model.createTable(), true);
}

void Test_ORMObject::test_save()
{
    QTime time = QTime::currentTime();
    MyModel model;
    model.setnameBool(true);
    model.setnameBlob(QByteArray());
    model.setnameChar('A');
    model.setnameDate(QDate(2013, 03, 26));
    model.setnameDatetime(QDateTime(QDate(2013, 03, 26), time));
    model.setnamedouble(0.1);
    model.setnameInt(10);
    model.setnameLonglong(1234567890);
    model.setnameString("Hello world!");
    model.setnameTime(time);
    model.setnameUint(60000);
    model.setnameUlonglong(123456789123456789);
    QCOMPARE(model.save(), true);
    QSqlQuery query = db.exec("SELECT * FROM MyModel;");
    query.next();
    for(int i = 0; i < query.size(); i++)
        if(query.record().fieldName(i) != "id")
            QCOMPARE(query.value(i), model.property(query.record().fieldName(i).toLocal8Bit().constData()));
        else
            QCOMPARE(query.value(i).toInt(), model.getId());
}

QTEST_MAIN(Test_ORMObject)

#include "tst_test_ormobject.moc"
