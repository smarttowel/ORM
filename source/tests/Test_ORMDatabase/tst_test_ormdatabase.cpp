#include <QString>
#include <QtTest>
#include "ormdatabase.cpp"
#include "mysqladapter.cpp"
#include "ormabstractadapter.cpp"

class Test_ORMDatabase : public QObject
{
    Q_OBJECT
    
public:
    Test_ORMDatabase();
    
private Q_SLOTS:
    void constructor();
    void test_addDatabase();
};

Test_ORMDatabase::Test_ORMDatabase()
{
}

void Test_ORMDatabase::constructor()
{
    ORMDatabase db("QMYSQL");
    QCOMPARE(db.driverName(), QString("QMYSQL"));
}

void Test_ORMDatabase::test_addDatabase()
{
    ORMDatabase db;
    QCOMPARE(db.driverName(), QString(""));
    db = ORMDatabase::addORMDatabase("QMYSQL");
    QCOMPARE(db.driverName(), QString("QMYSQL"));
}

QTEST_APPLESS_MAIN(Test_ORMDatabase)

#include "tst_test_ormdatabase.moc"
