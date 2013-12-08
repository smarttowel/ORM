#include <QObject>
#include <QString>
#include <QtTest>
#include <QSqlRecord>
#include <QSqlField>
#include <QList>
#include <QFile>
#include <QSqlDriver>
#include "ormdatabase.cpp"
#include "adapters/ormabstractadapter.cpp"
#include "adapters/sqladapter.cpp"
#include "adapters/mysqladapter.cpp"
#include "adapters/sqliteadapter.cpp"
#include "adapters/postgresqladapter.cpp"
#include "ormobject.h"
#include "ormwhere.cpp"
#include "ormgroupby.cpp"
#include "ormorderby.cpp"
#include "ormlogger.cpp"

/* Define DBMS in which will be executed tests
 * 0 - MySql
 * 1 - SQLite
 * 2 - PostgreSQL
*/
#define DBMS 1

#define CLEAR_LIST \
    for(int i = 0; i < list.size(); i++) \
        delete list.takeAt(i);

class MyModel : public ORMObject<MyModel>
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

class Car : public ORMObject<Car>
{
    Q_OBJECT
    ORM_PROPERTY(QString, Number)

public:
    Car() {}
};

class DriverLicense : public ORMObject<DriverLicense>
{
    Q_OBJECT
    ORM_PROPERTY(int, Number)

public:
    DriverLicense() {}
};

class CarDriver : public ORMObject<CarDriver>
{
    Q_OBJECT
    ORM_HAS_ONE(DriverLicense)
    ORM_HAS_MANY(Car)
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
    void test_createTable();
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
    void test_max();
    void test_min();
    void test_sum();
    void test_ORM_HAS_ONE();
    void test_ORM_HAS_MANY();
    void test_includes();
    void test_pluck();
    void test_toString();
    void test_hasUnsavedChanges();
    void test_setNull();
    void test_dropTable();
};

Test_ORMObject::Test_ORMObject()
{
#if DBMS == 0
    db = ORMDatabase::addORMDatabase("QMYSQL");
    db.setUserName("root");
    db.setHostName("localhost");
    db.open();
    db.createDatabase("Test_ORMDatabase");
#elif DBMS == 1
    db = ORMDatabase::addORMDatabase("QSQLITE");
    db.setDatabaseName("Test_ORMDatabase");
    db.open();
#elif DBMS == 2
    db = ORMDatabase::addORMDatabase("QPSQL");
    db.setUserName("postgres");
    db.setPassword("postgres");
    db.setHostName("localhost");
    db.open();
#endif
    //qDebug() << db.driverName();
}

Test_ORMObject::~Test_ORMObject()
{
#if DBMS == 0
    db.exec("DROP DATABASE Test_ORMDatabase;");
#elif DBMS == 1
    QFile file("Test_ORMDatabase");
    file.remove();
#endif
}

void Test_ORMObject::test_createTable()
{
    MyModel model;
    CarDriver driver;
    DriverLicense license;
    Car car;
    QCOMPARE(model.createTable(), true);
    QCOMPARE(model.createTable(), false);
    QCOMPARE(driver.createTable(), true);
    QCOMPARE(license.createTable(), true);
    QCOMPARE(car.createTable(), true);
    QCOMPARE(driver.createTableRelation(ORMAbstractAdapter::HasOne, "DriverLicense"), true);
    QCOMPARE(driver.createTableRelation(ORMAbstractAdapter::HasMany, "Car"), true);
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
    QCOMPARE(model.save(), true);
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
    MyModel model, model2;
    model2.setnameInt(10);
    QCOMPARE(model2.update(), false);
    QCOMPARE(model.findAll().isEmpty(), true);
    model2.save();
    QCOMPARE(model.findAll().isEmpty(), false);
    model.setId(model2.getId());
    model.setnameInt(15);
    QCOMPARE(model.update(), true);
    QCOMPARE(model2.findAll().isEmpty(), false);
    QCOMPARE(model2.first()->getnameInt(), 15);
}

void Test_ORMObject::test_find()
{
    db.exec("DELETE FROM MyModel;");
    MyModel model;
    MyModel *pointer;
    QCOMPARE(model.getId(), -1);
    QCOMPARE(model.save(), true);
    int id = model.getId();
    QVERIFY(id >= 0);
    QCOMPARE((pointer = model.find(id))->getId(), id);
    delete pointer;
    QVERIFY(model.find(13423) == 0);
    QCOMPARE(model.getId(), id);
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
    QCOMPARE(resultModel.findAll().isEmpty(), true);
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
    QList<MyModel*> list;
    list = resultModel.findAll();
    QCOMPARE(list.size(), 4);
    CLEAR_LIST
    list = resultModel.findAll(ORMGroupBy("id"));
    QCOMPARE(list.size(), 4);
    CLEAR_LIST
#if DBMS != 2
    list = resultModel.findAll(ORMGroupBy("nameInt"));
    QCOMPARE(list.size(), 3);
    CLEAR_LIST
    list = resultModel.findAll(ORMGroupBy("nameInt") && ORMGroupBy("nameString"));
    QCOMPARE(list.size(), 4);
    model1.updateProperty("nameString", "a");
    model2.updateProperty("nameInt", "0");
    CLEAR_LIST
    list = resultModel.findAll(ORMGroupBy("nameInt") && ORMGroupBy("nameString"));
    QCOMPARE(list.size(), 2);
    CLEAR_LIST
#endif
    list = resultModel.findAll(ORMGroupBy(), ORMOrderBy("nameInt", ORMOrderBy::Descending));
    QCOMPARE(list.first()->getnameInt(), 2);
    CLEAR_LIST
#if DBMS != 2
    list = resultModel.findAll(ORMGroupBy("nameInt") && ORMGroupBy("nameString"), ORMOrderBy("nameInt", ORMOrderBy::Descending));
    QCOMPARE(list.size(), 2);
    QCOMPARE(list.first()->getnameInt(), 2);
    CLEAR_LIST
#endif
}

void Test_ORMObject::test_findByValue()
{
    db.exec("DELETE FROM MyModel;");
    MyModel model;
    model.setnameInt(15);
    model.setnameString("a");
    model.save();
    QList<MyModel*> list;
    list = model.findBy("nameInt", QVariant(15));
    QCOMPARE(list.first()->getnameInt(), 15);
    MyModel model2;
    list = model2.findBy("nameString", QVariant("sdjkfhsjk"));
    QCOMPARE(model2.getId(), -1);
    QCOMPARE(list.size(), 0);
    CLEAR_LIST
#if DBMS != 2
    list = model2.findBy("nameInt", 15, ORMGroupBy("nameInt"));
    QCOMPARE(list.size(), 1);
    CLEAR_LIST
#endif
    MyModel model3;
    model3.setnameInt(15);
    model3.setnameString("b");
    model3.save();
#if DBMS != 2
    list = model2.findBy("nameInt", 15, ORMGroupBy("nameInt"));
    QCOMPARE(list.size(), 1);
    CLEAR_LIST
#endif
    list = model3.findBy("nameInt", 15, ORMGroupBy(), ORMOrderBy("id", ORMOrderBy::Ascending));
    QCOMPARE(list.first()->getnameString(), QString("a"));
    CLEAR_LIST
    list = model3.findBy("nameInt", 15, ORMGroupBy(), ORMOrderBy("id", ORMOrderBy::Descending));
    QCOMPARE(list.first()->getnameString(), QString("b"));
    CLEAR_LIST
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
    QList<MyModel*> list;
    list = resultModel.findBy("nameInt", vector);
    QCOMPARE(list.size(), 2);
    QCOMPARE(list.first()->getId(), model1.getId());
    vector.clear();
    CLEAR_LIST
    list = resultModel.findBy("nameInt", vector);
    QCOMPARE(list.isEmpty(), true);
    vector.append(20);
    list = resultModel.findBy("nameInt", vector);
    QCOMPARE(list.isEmpty(), true);
    CLEAR_LIST
#if DBMS != 2
    vector.clear();
    vector.append(10);
    vector.append(11);
    model3.updateProperty("nameInt", 10);
    list = resultModel.findBy("nameInt", vector, ORMGroupBy("nameInt"));
    QCOMPARE(list.size(), 2);
    CLEAR_LIST
    list = resultModel.findBy("nameInt", vector, ORMGroupBy("nameInt"), ORMOrderBy("nameInt", ORMOrderBy::Descending));
    QCOMPARE(list.size(), 2);
    QCOMPARE(list.first()->getnameInt(), 11);
    CLEAR_LIST
#endif
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
    QList<MyModel*> list;
    list = resultModel.findBy(findHash);
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
    CLEAR_LIST
    list = resultModel.findBy(findHash);
    QCOMPARE(list.isEmpty(), true);
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
    QList<MyModel*> list;
    list = resultModel.where(ORMWhere("nameInt", ORMWhere::Equals, 3));
    QCOMPARE(list.size(), 1);
    QCOMPARE(list.first()->getnameInt(), 3);
    QVERIFY(resultModel.getId() < 0);
    CLEAR_LIST
    list = resultModel.where(ORMWhere("nameString", ORMWhere::Equals, "Hello"));
    QCOMPARE(list.isEmpty(), true);
    CLEAR_LIST
    list = resultModel.where(ORMWhere("nameInt", ORMWhere::Equals, 1) && ORMWhere("nameInt", ORMWhere::Equals, 5));
    QCOMPARE(list.isEmpty(), true);
    CLEAR_LIST
    list = resultModel.where(ORMWhere("nameInt", ORMWhere::Equals, 1) || ORMWhere("nameInt", ORMWhere::LessThan, 5));
    QCOMPARE(list.size(), 2);
    CLEAR_LIST
    list = resultModel.where(ORMWhere("nameInt", ORMWhere::Equals, 1) || ORMWhere("nameString", ORMWhere::IsNull, ""));
    QCOMPARE(list.size(), 3);
    CLEAR_LIST
    list = resultModel.where(ORMWhere("nameInt", ORMWhere::Equals, 1) &&
                      (ORMWhere("nameString", ORMWhere::IsNull, "") || ORMWhere("nameInt", ORMWhere::GreaterThan, 3)));
    QCOMPARE(list.size(), 1);
    QCOMPARE(list.first()->getnameInt(), 1);
    CLEAR_LIST
#if DBMS != 2
    model3.updateProperty("nameInt", 3);
    list = resultModel.where(ORMWhere("nameInt", ORMWhere::Equals, 3), ORMGroupBy("nameInt"));
    QCOMPARE(list.size(), 1);
    CLEAR_LIST
    list = resultModel.where(ORMWhere("nameInt", ORMWhere::Equals, 3) || ORMWhere("nameInt", ORMWhere::Equals, 1),
                               ORMGroupBy("nameInt"));
    QCOMPARE(list.size(), 2);
    CLEAR_LIST
    list = resultModel.where(ORMWhere("nameInt", ORMWhere::Equals, 3) || ORMWhere("nameInt", ORMWhere::Equals, 1),
                               ORMGroupBy("nameInt"), ORMOrderBy("nameInt", ORMOrderBy::Descending));
    QCOMPARE(list.first()->getnameInt(), 3);
    CLEAR_LIST
#endif
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
    QCOMPARE(model2.first()->getnameString(), QString("Hello"));
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
    QList<MyModel*> list = resultModel.findAll();
    QCOMPARE(list.isEmpty(), false);
    QCOMPARE(list.size(), 3);
    QCOMPARE(resultModel.find(id)->getId(), id);
    QCOMPARE(model.remove(), true);
    QVERIFY(resultModel.find(id) == 0);
    QCOMPARE(resultModel.find(id2)->getId(), id2);
    QCOMPARE(model2.remove(), true);
    list = resultModel.findAll();
    QCOMPARE(list.isEmpty(), false);
    QCOMPARE(list.size(), 1);
    QVERIFY(resultModel.find(id2) == 0);
    QCOMPARE(model3.remove(), true);
    QCOMPARE(resultModel.findAll().isEmpty(), true);
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
    QCOMPARE(resultModel.findAll().isEmpty(), false);
    QCOMPARE(resultModel.removeBy(ORMWhere("nameString", ORMWhere::Equals, "foo") || ORMWhere("nameInt", ORMWhere::Equals, "15")), true);
    QCOMPARE(resultModel.findAll().isEmpty(), true);
}

void Test_ORMObject::test_removeAll()
{
    db.exec("DELETE FROM MyModel;");
    MyModel model1, model2, model3;
    model1.save();
    model2.save();
    model3.save();
    QList<MyModel*> list;
    list = model1.findAll();
    QCOMPARE(list.size(), 3);
    model1.removeAll();
    QCOMPARE(model1.findAll().isEmpty(), true);
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
    model1.setnameInt(1);
    model2.setnameInt(1);
    model3.setnameInt(1);
    model1.setnameString("Hello!");
    model2.setnameString("Hello!");
    model3.setnameString("Hello!");
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
    db.exec("DELETE FROM MyModel;");
    MyModel model4, model5, model6;
    model4.setnameInt(10);
    model4.setnameString("abc");
    model5.setnameInt(15);
    model5.setnameString("abc");
    model6.setnameInt(20);
    model6.setnameString("cba");
    model4.save();
    model5.save();
    model6.save();
    QCOMPARE(model4.average("nameInt", ORMWhere("nameString", ORMWhere::Equals, "abc")), double(12.5));
    QCOMPARE(model4.average("nameInt", ORMWhere("nameString", ORMWhere::Equals, "abc")
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
    db.exec("DELETE FROM MyModel;");
    MyModel model4, model5, model6;
    model4.setnameInt(10);
    model4.setnameString("abc");
    model5.setnameInt(15);
    model5.setnameString("abc");
    model6.setnameInt(20);
    model6.setnameString("cba");
    model4.save();
    model5.save();
    model6.save();
    QCOMPARE(model4.maximum("nameInt", ORMWhere("nameString", ORMWhere::Equals, "abc")), double(15));
    QCOMPARE(model4.maximum("nameInt", ORMWhere("nameString", ORMWhere::Equals, "abc")
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
    db.exec("DELETE FROM MyModel;");
    MyModel model4, model5, model6;
    model4.setnameInt(10);
    model4.setnameString("abc");
    model5.setnameInt(15);
    model5.setnameString("abc");
    model6.setnameInt(-20);
    model6.setnameString("cba");
    model4.save();
    model5.save();
    model6.save();
    QCOMPARE(model4.minimum("nameInt", ORMWhere("nameString", ORMWhere::Equals, "abc")), double(10));
    QCOMPARE(model4.minimum("nameInt", ORMWhere("nameString", ORMWhere::Equals, "abc")
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
    db.exec("DELETE FROM MyModel;");
    MyModel model4, model5, model6;
    model4.setnameInt(10);
    model4.setnameString("abc");
    model5.setnameInt(15);
    model5.setnameString("abc");
    model6.setnameInt(-20);
    model6.setnameString("cba");
    model4.save();
    model5.save();
    model6.save();
    QCOMPARE(model4.sum("nameInt", ORMWhere("nameString", ORMWhere::Equals, "abc")), double(25));
    QCOMPARE(model4.sum("nameInt", ORMWhere("nameString", ORMWhere::Equals, "abc")
                            || ORMWhere("nameString", ORMWhere::Equals, "cba")), double(5));
}

void Test_ORMObject::test_first()
{
    db.exec("DELETE FROM MyModel;");
    MyModel model, model2, model3;
    QCOMPARE(model.save(), true);
    QCOMPARE(model2.save(), true);
    QCOMPARE(model3.first()->getId(), model.getId());
    db.exec("DELETE FROM MyModel;");
    QVERIFY(model.first() == 0);
}

void Test_ORMObject::test_last()
{
    db.exec("DELETE FROM MyModel;");
    MyModel model, model2, model3;
    QCOMPARE(model.save(), true);
    QCOMPARE(model2.save(), true);
    QCOMPARE(model3.last()->getId(), model2.getId());
    db.exec("DELETE FROM MyModel;");
    QVERIFY(model.last() == 0);
}

void Test_ORMObject::test_ORM_HAS_ONE()
{
    CarDriver driver1, driver2;
    DriverLicense license, *pointer;
    driver1.removeAll();
    license.removeAll();
    driver1.setName("Alex");
    driver2.setName("Paul");
    driver1.save();
    driver2.save();
    QVERIFY(driver1.getDriverLicense() == 0);
    QHash<QString, QVariant> info;
    info.insert("Number", 123);
    QCOMPARE((pointer = driver1.createDriverLicense(info))->getNumber(), 123);
    delete pointer;
    QCOMPARE((pointer = driver1.getDriverLicense())->getNumber(), 123);
    delete pointer;
    QVERIFY(driver2.getDriverLicense() == 0);
    license.setNumber(456);
    license.save();
    driver2.setDriverLicense(license.getId());
    QCOMPARE((pointer = driver2.getDriverLicense())->getNumber(), 456);
    delete pointer;
    int idDr2Lic = (pointer = driver2.getDriverLicense())->getId();
    delete pointer;
    QCOMPARE(license.exists(idDr2Lic), true);
    QCOMPARE((pointer = driver2.getDriverLicense())->remove(), true);
    delete pointer;
    QCOMPARE(license.exists(idDr2Lic), false);
    QVERIFY(driver2.getDriverLicense() == 0);
    QVERIFY((pointer = driver1.getDriverLicense())->getId() >= 0);
    delete pointer;
}

void Test_ORMObject::test_ORM_HAS_MANY()
{
    CarDriver driver1, driver2;
    Car car1, car2, car3, *pointer;
    driver1.removeAll();
    car1.removeAll();
    driver1.setName("Alex");
    driver2.setName("Paul");
    driver1.save();
    driver2.save();
    QVERIFY(driver1.getAllCar().size() == 0);
    QHash<QString, QVariant> info;
    info.insert("Number", "123");
    QCOMPARE((pointer = driver1.createCar(info))->getNumber(), QString("123"));
    delete pointer;
    info.clear();
    info.insert("Number", "456");
    QCOMPARE((pointer = driver1.createCar(info))->getNumber(), QString("456"));
    delete pointer;
    QList<Car*> list = driver1.getAllCar();
    QCOMPARE(list.size(), 2);
    QCOMPARE(list.first()->getNumber(), QString("123"));
    QCOMPARE(list.value(1)->getNumber(), QString("456"));
    QVERIFY(driver2.getAllCar().size() == 0);
    car1.setNumber("111");
    car2.setNumber("111");
    car3.setNumber("222");
    car1.save();
    car2.save();
    car3.save();
    driver2.addCar(car1.getId());
    driver2.addCar(car2.getId());
    driver2.addCar(car3.getId());
    driver2.removeAllCar();
    QCOMPARE(driver2.getAllCar().size(), 0);
    QCOMPARE(car1.exists(), true);
    QCOMPARE(car2.exists(), true);
    QCOMPARE(car3.exists(), true);
    driver2.addCar(car1.getId());
    driver2.addCar(car2.getId());
    driver2.addCar(car3.getId());
    CLEAR_LIST
    list = driver2.getAllCar();
    QCOMPARE(list.size(), 3);
    QCOMPARE(list.first()->getNumber(), QString("111"));
    QCOMPARE(list.value(1)->getNumber(), QString("111"));
    QCOMPARE(list.value(2)->getNumber(), QString("222"));
    CLEAR_LIST
#if DBMS != 2
    list = driver2.findCarWhere(ORMWhere("Number", ORMWhere::Equals, "111") || ORMWhere("Number", ORMWhere::Equals, "222"),
                                ORMGroupBy("Number"), ORMOrderBy("Number", ORMOrderBy::Descending));
    QCOMPARE(list.size(), 2);
    QCOMPARE(list.first()->getNumber(), QString("222"));
    QCOMPARE(list.value(1)->getNumber(), QString("111"));
    CLEAR_LIST
    list = driver2.getAllCar(ORMGroupBy("Number"), ORMOrderBy("Number", ORMOrderBy::Descending));
    QCOMPARE(list.size(), 2);
    QCOMPARE(list.first()->getNumber(), QString("222"));
    QCOMPARE(list.value(1)->getNumber(), QString("111"));
#endif
    QCOMPARE(car1.remove(), true);
    QCOMPARE((list = driver2.getAllCar()).size(), 2);
    CLEAR_LIST
    QCOMPARE(car1.exists(car1.getId()), false);
}

void Test_ORMObject::test_includes()
{
    Car car1, car2, car3;
    DriverLicense license1, license2;
    CarDriver driver1, driver2;
    car1.removeAll();
    license1.removeAll();
    driver1.removeAll();
    driver1.setName("Peter");
    driver2.setName("Paul");
    car1.setNumber("1");
    car2.setNumber("2");
    car3.setNumber("3");
    license1.setNumber(123);
    license2.setNumber(456);
    driver1.save();
    driver2.save();
    car1.save();
    car2.save();
    car3.save();
    license1.save();
    license2.save();
    driver1.setDriverLicense(license1.getId());
    driver2.setDriverLicense(license2.getId());
    driver1.addCar(car1.getId());
    driver2.addCar(car2.getId());
    driver2.addCar(car3.getId());
    QStringList models;
    models << "DriverLicense" << "Car";
    QList<CarDriver*> list = driver1.includes(models);
    QCOMPARE(list.size(), 2);
    QCOMPARE(list.value(0)->getCarAfterIncludes().size(), 1);
    QCOMPARE(list.value(0)->getName(), QString("Peter"));
    QCOMPARE(list.value(1)->getName(), QString("Paul"));
    QCOMPARE(list.value(0)->getCarAfterIncludes().first()->getNumber(), QString("1"));
    QCOMPARE(list.value(1)->getCarAfterIncludes().size(), 2);
    QCOMPARE(list.value(1)->getCarAfterIncludes().first()->getNumber(), QString("2"));
    QCOMPARE(list.value(1)->getCarAfterIncludes().value(1)->getNumber(), QString("3"));
    QCOMPARE(list.first()->getDriverLicense()->getNumber(), 123);
    QCOMPARE(list.value(1)->getDriverLicense()->getNumber(), 456);
    car1.remove();
    car2.remove();
    CLEAR_LIST
    list = driver1.includes(models);
    QCOMPARE(list.first()->getCarAfterIncludes().size(), 0);
    QCOMPARE(list.value(1)->getCarAfterIncludes().size(), 1);
    models.removeAt(1);
    CLEAR_LIST
    list = driver1.includes(models);
    QCOMPARE(list.first()->getCarAfterIncludes().size(), 0);
    QCOMPARE(list.value(1)->getCarAfterIncludes().size(), 0);
    CLEAR_LIST
    list = driver1.includes(models, ORMWhere("id", ORMWhere::Equals, driver1.getId()));
    QCOMPARE(list.size(), 1);
    QCOMPARE(list.first()->getName(), QString("Peter"));
}

void Test_ORMObject::test_pluck()
{
    MyModel model1, model2, model3;
    model1.removeAll();
    model1.setnameInt(-2);
    model1.setnameString("abc");
    model2.setnameInt(1);
    model2.setnameString("def");
    model3.setnameInt(5);
    model3.setnameString("ghi");
    model1.save();
    model2.save();
    model3.save();
    QList<QVariant> list = model1.pluck("id");
    QCOMPARE(list.size(), 3);
    QCOMPARE(list.value(0).toInt(), model1.getId());
    QCOMPARE(list.value(1).toInt(), model2.getId());
    QCOMPARE(list.value(2).toInt(), model3.getId());
    list = model1.pluck("nameInt", ORMWhere("nameString", ORMWhere::Equals, "abc") ||
                        ORMWhere("nameString", ORMWhere::Equals, "ghi"));
    QCOMPARE(list.size(), 2);
    QCOMPARE(list.value(0).toInt(), -2);
    QCOMPARE(list.value(1).toInt(), 5);
    model1.setnameString("def");
    model1.update();
    list = model1.pluck("nameString", ORMWhere(), ORMGroupBy("nameString"));
    QCOMPARE(list.size(), 2);
}

void Test_ORMObject::test_toString()
{
    Car model;
    model.setNumber("123");
    model.setId(10);
    QString result = "Car\n    id : 10\n    Number : 123\n";
    QCOMPARE(model.toString(), result);
}

void Test_ORMObject::test_hasUnsavedChanges()
{
    MyModel model;
    model.removeAll();
    QCOMPARE(model.getId(), -1);
    QCOMPARE(model.hasUnsavedChanges(), false);
    model.setnameString("123ABC");
    QCOMPARE(model.hasUnsavedChanges(), true);
    model.setnameString("1234ABC");
    QCOMPARE(model.hasUnsavedChanges(), true);
    model.setnameInt(123);
    QCOMPARE(model.hasUnsavedChanges(), true);
    QCOMPARE(model.save(), true);
    QVERIFY(model.getId() >= 0);
    QCOMPARE(model.hasUnsavedChanges(), false);
    model.setnameString("123ABC");
    QCOMPARE(model.hasUnsavedChanges(), true);
    QCOMPARE(model.update(), true);
    QCOMPARE(model.hasUnsavedChanges(), false);
}

void Test_ORMObject::test_setNull()
{
    Car car;
    CarDriver driver;
    car.removeAll();
    driver.removeAll();
    car.setNumber("123abc");
    car.save();
    driver.setName("Bob");
    driver.save();
    driver.addCar(car.getId());
    QCOMPARE(driver.getAllCar().size(), 1);
    driver.removeCar(car.getId());
    QCOMPARE(driver.getAllCar().size(), 0);
}

void Test_ORMObject::test_dropTable()
{
    MyModel model;
    CarDriver driver;
    DriverLicense license;
    Car car;
    QCOMPARE(model.dropTable(), true);
    QCOMPARE(model.dropTable(), false);
    QCOMPARE(license.dropTable(), true);
    QCOMPARE(car.dropTable(), true);
    QCOMPARE(driver.dropTable(), true);
}

QTEST_MAIN(Test_ORMObject)

#include "tst_test_ormobject.moc"
