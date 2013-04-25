#include <QObject>
#include <QString>
#include <QtTest>
#include <QSqlRecord>
#include <QSqlField>
#include <QList>
#include "ormdatabase.cpp"
#include "mysqladapter.cpp"
#include "ormabstractadapter.cpp"
#include "ormobject.cpp"
#include "ormwhere.cpp"
#include "ormgroupby.cpp"
#include "ormorderby.cpp"

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

class DriverLicense : public ORMObject
{
    Q_OBJECT
    ORM_PROPERTY(int, Number)

public:
    DriverLicense() {}
};

class CarDriver : public ORMObject
{
    Q_OBJECT
    ORM_HAS_ONE(DriverLicense)
    ORM_PROPERTY(QString, Name)

public:
    CarDriver() {}
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
    void test_listSize();
    void test_save();
    void test_find();
    void test_findAll();
    void test_first();
    void test_last();
    void test_findByValue();
    void test_findByValues();
    void test_findByParams();
    void test_where();
    void test_update();
    void test_updateProperty();
    void test_exists();
    void test_existsById();
    void test_existsByWhere();
    void test_remove();
    void test_removeBy();
    void test_removeAll();
    void test_count();
    void test_countByFieldName();
    void test_countByWhere();
    void test_average();
    void test_averageByWhere();
    void test_max();
    void test_maxByWhere();
    void test_min();
    void test_minByWhere();
    void test_sum();
    void test_sumByWhere();
    void test_ORM_HAS_ONE();
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

void Test_ORMObject::test_listSize()
{
    db.exec("DELETE FROM MyModel;");
    MyModel model;
    QCOMPARE(model.listSize(), 0);
    model.save();
    QCOMPARE(model.listSize(), 0);
    model.first();
    QCOMPARE(model.listSize(), 1);
}

void Test_ORMObject::test_save()
{
    db.exec("DELETE FROM MyModel;");
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
    QCOMPARE(model.listSize(), 0);
    QCOMPARE(model.save(), true);
    QCOMPARE(model.listSize(), 0);
    QSqlQuery query = db.exec("SELECT * FROM MyModel;");
    query.next();
    for(int i = 0; i < query.size(); i++)
        if(query.record().fieldName(i) != "id")
            QCOMPARE(query.value(i), model.property(query.record().fieldName(i).toLocal8Bit().constData()));
        else
            QCOMPARE(query.value(i).toInt(), model.getId());
}

void Test_ORMObject::test_update()
{
    db.exec("DELETE FROM MyModel;");
    MyModel model2, model3;
    model2.setnameInt(10);
    QCOMPARE(model2.update(), false);
    QCOMPARE(model3.findAll(), false);
    model2.save();
    QCOMPARE(model3.findAll(), true);
    model3.setnameInt(15);
    QCOMPARE(model3.update(), true);
    QCOMPARE(model2.findAll(), true);
    QCOMPARE(model2.listSize(), 1);
    QCOMPARE(model2.getnameInt(), 15);
}

void Test_ORMObject::test_find()
{
    db.exec("DELETE FROM MyModel;");
    MyModel model;
    QCOMPARE(model.getId(), -1);
    QCOMPARE(model.save(), true);
    int id = model.getId();
    QVERIFY(id >= 0);
    QCOMPARE(model.listSize(), 0);
    QCOMPARE(model.find(id), true);
    QVERIFY(model.listSize() != 0);
    QCOMPARE(model.getId(), id);
    QCOMPARE(model.find(13423), false);
    QCOMPARE(model.getId(), id);
    QVERIFY(model.listSize() != 0);
    QSqlQuery query = db.exec(QString("SELECT * FROM MyModel WHERE id = %1;")
                              .arg(id));
    query.next();
    for(int i = 0; i < query.size(); i++)
        if(query.record().fieldName(i) != "id")
            QCOMPARE(query.value(i), model.property(query.record().fieldName(i).toLocal8Bit().constData()));
        else
            QCOMPARE(query.value(i).toInt(), model.getId());
}

void Test_ORMObject::test_findAll()
{
    db.exec("DELETE FROM MyModel;");
    MyModel model, model1, model2, model3, resultModel;
    QCOMPARE(resultModel.findAll(), false);
    model.setnameInt(0);
    model.setnameString("a");
    model1.setnameInt(0);
    model1.setnameString("b");
    model2.setnameInt(1);
    model2.setnameString("a");
    model3.setnameInt(2);
    model3.setnameString("c");
    model.save();
    model1.save();
    model2.save();
    model3.save();
    QCOMPARE(resultModel.findAll(), true);
    QCOMPARE(resultModel.listSize(), 4);
    QCOMPARE(resultModel.findAll(ORMGroupBy("id")), true);
    QCOMPARE(resultModel.listSize(), 4);
    QCOMPARE(resultModel.findAll(ORMGroupBy("nameInt")), true);
    QCOMPARE(resultModel.listSize(), 3);
    QCOMPARE(resultModel.findAll(ORMGroupBy("nameInt") && ORMGroupBy("nameString")), true);
    QCOMPARE(resultModel.listSize(), 4);
    model1.updateProperty("nameString", "a");
    model2.updateProperty("nameInt", "0");
    QCOMPARE(resultModel.findAll(ORMGroupBy("nameInt") && ORMGroupBy("nameString")), true);
    QCOMPARE(resultModel.listSize(), 2);
    QCOMPARE(resultModel.findAll(ORMGroupBy(), ORMOrderBy("nameInt", ORMOrderBy::Descending)), true);
    QCOMPARE(resultModel.getnameInt(), 2);
    QCOMPARE(resultModel.findAll(ORMGroupBy("nameInt") && ORMGroupBy("nameString"), ORMOrderBy("nameInt", ORMOrderBy::Descending)), true);
    QCOMPARE(resultModel.listSize(), 2);
    QCOMPARE(resultModel.getnameInt(), 2);
}

void Test_ORMObject::test_findByValue()
{
    db.exec("DELETE FROM MyModel;");
    MyModel model;
    model.setnameInt(15);
    model.setnameString("a");
    model.save();
    QCOMPARE(model.listSize(), 0);
    QCOMPARE(model.findBy("nameInt", QVariant(15)), true);
    QCOMPARE(model.getnameInt(), 15);
    QVERIFY(model.listSize() != 0);
    MyModel model2;
    QCOMPARE(model2.listSize(), 0);
    QCOMPARE(model2.findBy("nameString", QVariant("sdjkfhsjk")), false);
    QCOMPARE(model2.getId(), -1);
    QCOMPARE(model2.listSize(), 0);
    QCOMPARE(model2.findBy("nameInt", 15, ORMGroupBy("nameInt")), true);
    QCOMPARE(model2.listSize(), 1);
    MyModel model3;
    model3.setnameInt(15);
    model3.setnameString("b");
    model3.save();
    QCOMPARE(model2.findBy("nameInt", 15, ORMGroupBy("nameInt")), true);
    QCOMPARE(model2.listSize(), 1);
    QCOMPARE(model3.findBy("nameInt", 15, ORMGroupBy(), ORMOrderBy("id", ORMOrderBy::Ascending)), true);
    QCOMPARE(model3.getnameString(), QString("a"));
    QCOMPARE(model3.findBy("nameInt", 15, ORMGroupBy(), ORMOrderBy("id", ORMOrderBy::Descending)), true);
    QCOMPARE(model3.getnameString(), QString("b"));
}

void Test_ORMObject::test_findByValues()
{
    db.exec("DELETE FROM MyModel;");
    MyModel model1, model2, model3, resultModel;
    model1.setnameInt(10);
    model2.setnameInt(11);
    model3.setnameInt(12);
    model1.save();
    model2.save();
    model3.save();
    QVector<QVariant> vector;
    vector.append(10);
    vector.append(11);
    QCOMPARE(resultModel.findBy("nameInt", vector), true);
    QCOMPARE(resultModel.listSize(), 2);
    QCOMPARE(resultModel.getId(), model1.getId());
    vector.clear();
    QCOMPARE(resultModel.findBy("nameInt", vector), false);
    QCOMPARE(resultModel.getId(), model1.getId());
    vector.append(20);
    QCOMPARE(resultModel.findBy("nameInt", vector), false);
    vector.clear();
    vector.append(10);
    vector.append(11);
    model3.updateProperty("nameInt", 10);
    QCOMPARE(resultModel.findBy("nameInt", vector, ORMGroupBy("nameInt")), true);
    QCOMPARE(resultModel.listSize(), 2);
    QCOMPARE(resultModel.findBy("nameInt", vector, ORMGroupBy("nameInt"), ORMOrderBy("nameInt", ORMOrderBy::Descending)), true);
    QCOMPARE(resultModel.listSize(), 2);
    QCOMPARE(resultModel.getnameInt(), 11);
}

void Test_ORMObject::test_findByParams()
{
    db.exec("DELETE FROM MyModel;");
    MyModel model, model2, model3, resultModel;
    model.setnameString("Hello");
    model2.setnameInt(3);
    model3.setnamedouble(1.23);
    QCOMPARE(model.save(), true);
    QCOMPARE(model2.save(), true);
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
    findHash.clear();
    findHash.insert("id", QVariant(-100));
    QVERIFY(resultModel.listSize() != 0);
    QCOMPARE(resultModel.findBy(findHash), false);
    QVERIFY(resultModel.listSize() != 0);
}

void Test_ORMObject::test_where()
{
    db.exec("DELETE FROM MyModel;");
    MyModel model1, model2, model3, resultModel;
    model1.setnameInt(1);
    model2.setnameInt(3);
    model3.setnameInt(5);
    model1.save();
    model2.save();
    model3.save();
    QCOMPARE(resultModel.where(ORMWhere("nameInt", ORMWhere::Equals, 3)), true);
    QCOMPARE(resultModel.listSize(), 1);
    QCOMPARE(resultModel.getnameInt(), 3);
    QVERIFY(resultModel.getId() >= 0);
    QCOMPARE(resultModel.where(ORMWhere("nameString", ORMWhere::Equals, "Hello")), false);
    QVERIFY(resultModel.listSize() != 0);
    QCOMPARE(resultModel.where(ORMWhere("nameInt", ORMWhere::Equals, 1) && ORMWhere("nameInt", ORMWhere::Equals, 5)), false);
    QVERIFY(resultModel.listSize() != 0);
    QCOMPARE(resultModel.where(ORMWhere("nameInt", ORMWhere::Equals, 1) || ORMWhere("nameInt", ORMWhere::LessThan, 5)), true);
    QCOMPARE(resultModel.listSize(), 2);
    QCOMPARE(resultModel.where(ORMWhere("nameInt", ORMWhere::Equals, 1) || ORMWhere("nameString", ORMWhere::Equals, "")), true);
    QCOMPARE(resultModel.listSize(), 3);
    resultModel.where(ORMWhere("nameInt", ORMWhere::Equals, 1) &&
                      (ORMWhere("nameString", ORMWhere::Equals, "") || ORMWhere("nameInt", ORMWhere::GreaterThan, 3)));
    QCOMPARE(resultModel.listSize(), 1);
    QCOMPARE(resultModel.getnameInt(), 1);
    QCOMPARE(resultModel.getnameInt(), 1);
    model3.updateProperty("nameInt", 3);
    QCOMPARE(resultModel.where(ORMWhere("nameInt", ORMWhere::Equals, 3), ORMGroupBy("nameInt")), true);
    QCOMPARE(resultModel.listSize(), 1);
    QCOMPARE(resultModel.where(ORMWhere("nameInt", ORMWhere::Equals, 3) || ORMWhere("nameInt", ORMWhere::Equals, 1),
                               ORMGroupBy("nameInt")), true);
    QCOMPARE(resultModel.listSize(), 2);
    QCOMPARE(resultModel.where(ORMWhere("nameInt", ORMWhere::Equals, 3) || ORMWhere("nameInt", ORMWhere::Equals, 1),
                               ORMGroupBy("nameInt"), ORMOrderBy("nameInt", ORMOrderBy::Descending)), true);
    QCOMPARE(resultModel.getnameInt(), 3);
}

void Test_ORMObject::test_updateProperty()
{
    db.exec("DELETE FROM MyModel;");
    MyModel model, model2;
    model.setnameInt(10);
    model.save();
    QCOMPARE(model.updateProperty("nameString", "Hello"), true);
    QCOMPARE(model.updateProperty("ololoProperty", 123), false);
    QCOMPARE(model2.updateProperty("nameInt", 20), false);
    model2.first();
    QCOMPARE(model2.getnameString(), QString("Hello"));
}

void Test_ORMObject::test_exists()
{
    db.exec("DELETE FROM MyModel;");
    MyModel model;
    QCOMPARE(model.exists(), false);
    model.save();
    QCOMPARE(model.exists(), true);
    db.exec("DELETE FROM MyModel;");
    QCOMPARE(model.exists(), false);
}

void Test_ORMObject::test_existsById()
{
    db.exec("DELETE FROM MyModel;");
    MyModel model;
    QCOMPARE(model.exists(1), false);
    QCOMPARE(model.exists(model.getId()), false);
    model.save();
    QCOMPARE(model.exists(model.getId()), true);
    int id = model.getId();
    model.remove();
    QCOMPARE(model.exists(id), false);
}

void Test_ORMObject::test_existsByWhere()
{
    db.exec("DELETE FROM MyModel;");
    MyModel model, model2;
    model.setnameInt(10);
    model2.setnameInt(15);
    model.save();
    model2.save();
    QCOMPARE(model.exists(ORMWhere("nameInt", ORMWhere::Equals, 10) || ORMWhere("nameInt", ORMWhere::Equals, 15)), true);
    QCOMPARE(model.exists(ORMWhere("nameInt", ORMWhere::Equals, 10) && ORMWhere("nameInt", ORMWhere::Equals, 15)), false);
    QCOMPARE(model.exists(ORMWhere("nameInt", ORMWhere::LessThan, 13)), true);
    QCOMPARE(model.exists(ORMWhere("nameInt", ORMWhere::Equals, 20)), false);
}

void Test_ORMObject::test_remove()
{
    db.exec("DELETE FROM MyModel;");
    MyModel model, model2, model3, resultModel;
    model.save();
    model2.save();
    model3.save();
    int id = model.getId();
    int id2 = model2.getId();
    QCOMPARE(resultModel.findAll(), true);
    QCOMPARE(resultModel.listSize(), 3);
    QCOMPARE(resultModel.find(id), true);
    QCOMPARE(model.remove(), true);
    QCOMPARE(resultModel.find(id), false);
    QCOMPARE(resultModel.find(id2), true);
    QCOMPARE(model2.remove(), true);
    QCOMPARE(resultModel.findAll(), true);
    QCOMPARE(resultModel.listSize(), 1);
    QCOMPARE(resultModel.find(id2), false);
    QCOMPARE(model3.remove(), true);
    QCOMPARE(resultModel.findAll(), false);
}

void Test_ORMObject::test_removeBy()
{
    db.exec("DELETE FROM MyModel;");
    MyModel model1, model2, model3, resultModel;
    model1.setnameInt(10);
    model1.setnameString("foo");
    model2.setnameInt(15);
    model3.setnameString("foo");
    model1.save();
    model2.save();
    model3.save();
    QCOMPARE(resultModel.findAll(), true);
    QCOMPARE(resultModel.removeBy(ORMWhere("nameString", ORMWhere::Equals, "foo") || ORMWhere("nameInt", ORMWhere::Equals, "15")), true);
    QCOMPARE(resultModel.findAll(), false);
}

void Test_ORMObject::test_removeAll()
{
    db.exec("DELETE FROM MyModel;");
    MyModel model1, model2, model3;
    model1.save();
    model2.save();
    model3.save();
    QCOMPARE(model1.findAll(), true);
    QCOMPARE(model1.listSize(), 3);
    model1.removeAll();
    QCOMPARE(model1.findAll(), false);
}

void Test_ORMObject::test_count()
{
    db.exec("DELETE FROM MyModel;");
    MyModel model, model2, model3;
    QCOMPARE(model.count(), 0);
    model.save();
    QCOMPARE(model.count(), 1);
    model2.save();
    QCOMPARE(model.count(), 2);
    model3.save();
    QCOMPARE(model.count(), 3);
    model2.remove();
    model3.remove();
    QCOMPARE(model.count(), 1);
}

void Test_ORMObject::test_countByFieldName()
{
    db.exec("DELETE FROM MyModel;");
    MyModel model1, model2, model3;
    QCOMPARE(model2.count("nameint"), 0);
    model1.save();
    model2.save();
    model3.save();
    QCOMPARE(model1.count("nameInt"), 3);
    QCOMPARE(model2.count("nameString"), 3);
    QCOMPARE(model2.count("someField"), -1);
}

void Test_ORMObject::test_countByWhere()
{
    db.exec("DELETE FROM MyModel;");
    MyModel model1, model2, model3;
    model1.setnameInt(10);
    model2.setnameInt(15);
    model3.setnameString("abc");
    model3.setnameInt(20);
    model1.save();
    model2.save();
    model3.save();
    QCOMPARE(model1.count(ORMWhere("nameInt", ORMWhere::Equals, 10) || ORMWhere("nameInt", ORMWhere::Equals, 15)), 2);
    QCOMPARE(model1.count(ORMWhere("nameInt", ORMWhere::LessThan, 20) || ORMWhere("nameString", ORMWhere::Equals, "abc")), 3);
    QCOMPARE(model1.count(ORMWhere("nameInt", ORMWhere::GreaterOrEquals, 30)), 0);
    QCOMPARE(model1.count(ORMWhere("a", ORMWhere::GreaterOrEquals, 30)), -1);
}

void Test_ORMObject::test_average()
{
    db.exec("DELETE FROM MyModel;");
    MyModel model1, model2, model3;
    model1.setnameInt(10);
    model2.setnameInt(15);
    model3.setnameInt(20);
    model1.save();
    model2.save();
    model3.save();
    QCOMPARE(model1.average("nameInt"), double(15));
    model2.updateProperty("nameInt", 30);
    QCOMPARE(model1.average("nameInt"), double(20));
    model2.updateProperty("nameInt", -30);
    QCOMPARE(model1.average("nameInt"), double(0));
}

void Test_ORMObject::test_averageByWhere()
{
    db.exec("DELETE FROM MyModel;");
    MyModel model1, model2, model3;
    model1.setnameInt(10);
    model1.setnameString("abc");
    model2.setnameInt(15);
    model2.setnameString("abc");
    model3.setnameInt(20);
    model3.setnameString("cba");
    model1.save();
    model2.save();
    model3.save();
    QCOMPARE(model1.average("nameInt", ORMWhere("nameString", ORMWhere::Equals, "abc")), double(12.5));
    QCOMPARE(model1.average("nameInt", ORMWhere("nameString", ORMWhere::Equals, "abc")
                            || ORMWhere("nameString", ORMWhere::Equals, "cba")), double(15));
}

void Test_ORMObject::test_max()
{
    db.exec("DELETE FROM MyModel;");
    MyModel model1, model2, model3;
    model1.setnameInt(10);
    model2.setnameInt(15);
    model3.setnameInt(20);
    QCOMPARE(model1.maximum("nameInt"), double(0));
    model1.save();
    model2.save();
    model3.save();
    QCOMPARE(model1.maximum("nameInt"), double(20));
    model2.updateProperty("nameInt", -5);
    QCOMPARE(model1.maximum("nameInt"), double(20));
    model3.updateProperty("nameInt", 100);
    QCOMPARE(model1.maximum("nameInt"), double(100));
}

void Test_ORMObject::test_maxByWhere()
{
    db.exec("DELETE FROM MyModel;");
    MyModel model1, model2, model3;
    model1.setnameInt(10);
    model1.setnameString("abc");
    model2.setnameInt(15);
    model2.setnameString("abc");
    model3.setnameInt(20);
    model3.setnameString("cba");
    model1.save();
    model2.save();
    model3.save();
    QCOMPARE(model1.maximum("nameInt", ORMWhere("nameString", ORMWhere::Equals, "abc")), double(15));
    QCOMPARE(model1.maximum("nameInt", ORMWhere("nameString", ORMWhere::Equals, "abc")
                            || ORMWhere("nameString", ORMWhere::Equals, "cba")), double(20));
}

void Test_ORMObject::test_min()
{
    db.exec("DELETE FROM MyModel;");
    MyModel model1, model2, model3;
    model1.setnameInt(10);
    model2.setnameInt(15);
    model3.setnameInt(20);
    QCOMPARE(model1.minimum("nameInt"), double(0));
    model1.save();
    model2.save();
    model3.save();
    QCOMPARE(model1.minimum("nameInt"), double(10));
    model2.updateProperty("nameInt", -5);
    QCOMPARE(model1.minimum("nameInt"), double(-5));
    model3.updateProperty("nameInt", 100);
    QCOMPARE(model1.minimum("nameInt"), double(-5));
}

void Test_ORMObject::test_minByWhere()
{
    db.exec("DELETE FROM MyModel;");
    MyModel model1, model2, model3;
    model1.setnameInt(10);
    model1.setnameString("abc");
    model2.setnameInt(15);
    model2.setnameString("abc");
    model3.setnameInt(-20);
    model3.setnameString("cba");
    model1.save();
    model2.save();
    model3.save();
    QCOMPARE(model1.minimum("nameInt", ORMWhere("nameString", ORMWhere::Equals, "abc")), double(10));
    QCOMPARE(model1.minimum("nameInt", ORMWhere("nameString", ORMWhere::Equals, "abc")
                            || ORMWhere("nameString", ORMWhere::Equals, "cba")), double(-20));
}

void Test_ORMObject::test_sum()
{
    db.exec("DELETE FROM MyModel;");
    MyModel model1, model2, model3;
    model1.setnameInt(10);
    model2.setnameInt(15);
    model3.setnameInt(20);
    QCOMPARE(model1.sum("nameInt"), double(0));
    model1.save();
    model2.save();
    model3.save();
    QCOMPARE(model1.sum("nameInt"), double(45));
    model2.updateProperty("nameInt", -5);
    QCOMPARE(model1.sum("nameInt"), double(25));
    model3.updateProperty("nameInt", 100);
    QCOMPARE(model1.sum("nameInt"), double(105));
}

void Test_ORMObject::test_sumByWhere()
{
    db.exec("DELETE FROM MyModel;");
    MyModel model1, model2, model3;
    model1.setnameInt(10);
    model1.setnameString("abc");
    model2.setnameInt(15);
    model2.setnameString("abc");
    model3.setnameInt(-20);
    model3.setnameString("cba");
    model1.save();
    model2.save();
    model3.save();
    QCOMPARE(model1.sum("nameInt", ORMWhere("nameString", ORMWhere::Equals, "abc")), double(25));
    QCOMPARE(model1.sum("nameInt", ORMWhere("nameString", ORMWhere::Equals, "abc")
                            || ORMWhere("nameString", ORMWhere::Equals, "cba")), double(5));
}

void Test_ORMObject::test_first()
{
    db.exec("DELETE FROM MyModel;");
    MyModel model, model2, model3;
    QCOMPARE(model.save(), true);
    QCOMPARE(model2.save(), true);
    QCOMPARE(model3.first(), true);
    QCOMPARE(model3.getId(), model.getId());
    QVERIFY(model3.listSize() != 0);
    db.exec("DELETE FROM MyModel;");
    QCOMPARE(model.first(), false);
}

void Test_ORMObject::test_last()
{
    db.exec("DELETE FROM MyModel;");
    MyModel model, model2, model3;
    QCOMPARE(model.save(), true);
    QCOMPARE(model2.save(), true);
    QCOMPARE(model3.last(), true);
    QCOMPARE(model3.getId(), model2.getId());
    QVERIFY(model3.listSize() != 0);
    db.exec("DELETE FROM MyModel;");
    QCOMPARE(model.last(), false);
}

void Test_ORMObject::test_ORM_HAS_ONE()
{
    CarDriver driver1, driver2;
    DriverLicense license;
    QCOMPARE(driver1.createTable(), true);
    QCOMPARE(license.createTable(), true);
    driver1.setName("Alex");
    driver2.setName("Paul");
    driver1.save();
    driver2.save();
    QVERIFY(driver1.getDriverLicense() == 0);
    QHash<QString, QVariant> info;
    info.insert("Number", 123);
    QCOMPARE(driver1.createDriverLicense(info)->getNumber(), 123);
    QCOMPARE(driver1.getDriverLicense()->getNumber(), 123);
    QVERIFY(driver2.getDriverLicense() == 0);
    license.setNumber(456);
    license.save();
    driver2.setDriverLicense(license);
    QCOMPARE(driver2.getDriverLicense()->getNumber(), 456);
    int idDr2Lic = driver2.getDriverLicense()->getId();
    QCOMPARE(license.exists(idDr2Lic), true);
    driver2.removeDriverLicense();
    QCOMPARE(license.exists(idDr2Lic), true);
    QVERIFY(driver2.getDriverLicense() == 0);
    QVERIFY(driver1.getDriverLicense() != 0);
}

void Test_ORMObject::test_dropTable()
{
    MyModel model;
    QCOMPARE(model.dropTable(), true);
    QCOMPARE(model.dropTable(), false);
}

QTEST_MAIN(Test_ORMObject)

#include "tst_test_ormobject.moc"
