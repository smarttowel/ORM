#include <QString>
#include <QtTest>
#include "ormdatabase.cpp"
#include "mysqladapter.cpp"
#include "ormabstractadapter.cpp"

class Test_MySqlAdapter : public QObject
{
    Q_OBJECT
    
public:
    Test_MySqlAdapter();
    
private Q_SLOTS:
    void test_createDatabase();
};

Test_MySqlAdapter::Test_MySqlAdapter()
{
}

void Test_MySqlAdapter::test_createDatabase()
{
    MySqlAdapter adapter;
    QCOMPARE(adapter.createDatabase("ORM_TestDatabase"), false);
    QCOMPARE(adapter.lastQuery(), QString("CREATE DATABASE ORM_TestDatabase;"));
}

QTEST_APPLESS_MAIN(Test_MySqlAdapter)

#include "tst_test_mysqladapter.moc"
