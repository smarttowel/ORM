#include <QObject>
#include <QString>
#include <QtTest>
#include <QSqlRecord>
#include <QSqlField>
#include "ormdatabase.cpp"
#include "mysqladapter.cpp"
#include "ormabstractadapter.cpp"
#include "ormobject.cpp"
#include "ormwhere.cpp"

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
    void test_find();
    void test_first();
    void test_last();
    void test_findBy();
    void test_findBy2();
    void test_dropTable();
};

Test_ORMObject::Test_ORMObject()
{
    db = ORMDatabase::addORMDatabase("QMYSQL");
    db.setUserName("root");
    db.setHostName("localhost");
    db.createDatabase("Test_ORMDatabase");
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

void Test_ORMObject::test_find()
{
    MyModel model;
    QCOMPARE(model.getId(), -1);
    QCOMPARE(model.find(1), true);
    QCOMPARE(model.getId(), 1);
    QSqlQuery query = db.exec("SELECT * FROM MyModel WHERE id = 1;");
    query.next();
    for(int i = 0; i < query.size(); i++)
        if(query.record().fieldName(i) != "id")
            QCOMPARE(query.value(i), model.property(query.record().fieldName(i).toLocal8Bit().constData()));
        else
            QCOMPARE(query.value(i).toInt(), model.getId());
    QCOMPARE(model.find(-1), false);
    QCOMPARE(model.getId(), 1);
}

void Test_ORMObject::test_dropTable()
{
    MyModel model;
    QCOMPARE(model.dropTable(), true);
    QCOMPARE(model.dropTable(), false);
}

void Test_ORMObject::test_findBy()
{
    MyModel model;
    model.setnameInt(15);
    model.save();
    QCOMPARE(model.findBy("nameInt", QVariant(15)), true);
    QCOMPARE(model.getnameInt(), 15);
    MyModel model2;
    QCOMPARE(model2.findBy("nameString", QVariant("sdjkfhsjk")), false);
    QCOMPARE(model2.getId(), -1);
}

void Test_ORMObject::test_findBy2()
{
    db.exec("DELETE FROM MyModel;");
    MyModel model, model2, model3, resultModel;
    model.setnameString("Hello");
    QCOMPARE(model.save(), true);
    model2.setnameInt(3);
    QCOMPARE(model2.save(), true);
    model3.setnamedouble(1.23);
    QCOMPARE(model3.save(), true);
    QHash<QString, QVariant> findHash;
    findHash.insert("nameString", QVariant("Hello"));
    findHash.insert("nameInt", QVariant(3));
    findHash.insert("nameDouble", QVariant(1.23));
    findHash.insert("nameChar", QVariant('X'));
    QCOMPARE(resultModel.findBy(findHash), true);
    QList<MyModel*> list = resultModel.toList<MyModel>();
    QCOMPARE(list.size(), 3);
    for(int i = 0; i < list.size(); i++)
    {
        QVERIFY(model.getId() == list.value(i)->getId() || model2.getId() == list.value(i)->getId()
                || model3.getId() == list.value(i)->getId());
        if(list.value(i)->getId() == 1)
            QCOMPARE(list.value(i)->getnameString(), QString("Hello"));
        if(list.value(i)->getId() == 2)
        {
            QCOMPARE(list.value(i)->getnameInt(), 3);
            QCOMPARE(list.value(i)->getnameString().isEmpty(), true);
        }
        if(list.value(i)->getId() == 3)
            QCOMPARE(list.value(i)->getnamedouble(), 1.23);
    }
    findHash.clear();;
    findHash.insert("id", QVariant(-100));
    QCOMPARE(resultModel.findBy(findHash), false);
}

void Test_ORMObject::test_first()
{
    MyModel model;
    QCOMPARE(model.save(), true);
    QCOMPARE(model.first(), true);
    QCOMPARE(model.getId(), 1);
}

void Test_ORMObject::test_last()
{
    MyModel model;
    QCOMPARE(model.last(), true);
    QCOMPARE(model.getId(), 2);
    db.exec("DELETE FROM MyModel;");
    QCOMPARE(model.last(), false);
}

QTEST_MAIN(Test_ORMObject)

#include "tst_test_ormobject.moc"
