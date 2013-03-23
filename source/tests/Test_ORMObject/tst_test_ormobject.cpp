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
    
private Q_SLOTS:
    void test_CreateTable();
};

Test_ORMObject::Test_ORMObject()
{
}

void Test_ORMObject::test_CreateTable()
{
    ORMDatabase db("QMYSQL");
    db.setUserName("root");
    QCOMPARE(db.createDatabase("Test_ORMDatabase"), true);
    MyModel model;
    QCOMPARE(model.createTable(), true);
    db.exec("DROP DATABASE Test_ORMDatabase;");
}

QTEST_MAIN(Test_ORMObject)

#include "tst_test_ormobject.moc"
