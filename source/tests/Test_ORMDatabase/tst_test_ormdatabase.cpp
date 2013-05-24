#include <QString>
#include <QtTest>
#include "ormdatabase.cpp"
#include "mysqladapter.cpp"
#include "sqliteadapter.cpp"
#include "sqladapter.cpp"
#include "ormabstractadapter.cpp"

class Test_ORMDatabase : public QObject
{
    Q_OBJECT
    
public:
    Test_ORMDatabase();
    
private Q_SLOTS:
    void constructor();
    void test_addDatabase();
    void test_createDatabase();
    void test_lastQuery();
    void test_dropDatabase();
};

Test_ORMDatabase::Test_ORMDatabase()
{
}

void Test_ORMDatabase::constructor()
{
    {
        ORMDatabase db("QMYSQL");
        QCOMPARE(db.driverName(), QString("QMYSQL"));
    }
    ORMDatabase::removeDatabase("qt_sql_default_connection");
}

void Test_ORMDatabase::test_addDatabase()
{
    {
        ORMDatabase db;
        QCOMPARE(db.driverName(), QString(""));
        db = ORMDatabase::addORMDatabase("QMYSQL");
        QCOMPARE(db.driverName(), QString("QMYSQL"));
    }
    ORMDatabase::removeDatabase("qt_sql_default_connection");
}

void Test_ORMDatabase::test_createDatabase()
{
    {
        ORMDatabase db("QMYSQL");
        db.setUserName("root");
        db.open();
        QCOMPARE(db.createDatabase("Test_ORMDatabase"), true);
        QCOMPARE(db.dropDatabase("Test_ORMDatabase"), true);
        db.open();
        QCOMPARE(db.createDatabase("test"), false);
    }
    ORMDatabase::removeDatabase("qt_sql_default_connection");
}

void Test_ORMDatabase::test_lastQuery()
{
    {
        ORMDatabase db("QMYSQL");
        db.setUserName("root");
        QCOMPARE(db.lastQuery().isEmpty(), true);
        db.open();
        QCOMPARE(db.createDatabase("Test_ORMDatabase"), true);
        QCOMPARE(db.lastQuery().isEmpty(), false);
        QCOMPARE(db.dropDatabase("Test_ORMDatabase"), true);
    }
    ORMDatabase::removeDatabase("qt_sql_default_connection");
}

void Test_ORMDatabase::test_dropDatabase()
{
    {
        ORMDatabase db("QMYSQL");
        db.setUserName("root");
        db.open();
        QCOMPARE(db.createDatabase("Test_ORMDatabase"), true);
        QCOMPARE(db.dropDatabase("Test_ORMDatabase"), true);
        QCOMPARE(db.dropDatabase("Test_ORMDatabase"), false);
    }
}

QTEST_APPLESS_MAIN(Test_ORMDatabase)

#include "tst_test_ormdatabase.moc"
