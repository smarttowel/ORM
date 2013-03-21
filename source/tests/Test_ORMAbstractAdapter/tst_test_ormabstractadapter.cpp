#include <QString>
#include <QtTest>
#include "ormdatabase.cpp"
#include "mysqladapter.cpp"
#include "ormabstractadapter.cpp"

class Test_ORMAbstractAdapter : public QObject
{
    Q_OBJECT
    
public:
    Test_ORMAbstractAdapter();
    
private Q_SLOTS:
    void test_createDatabase();
};

Test_ORMAbstractAdapter::Test_ORMAbstractAdapter()
{
}

void Test_ORMAbstractAdapter::test_createDatabase()
{
    QVERIFY2(true, "Failure");
}

QTEST_APPLESS_MAIN(Test_ORMAbstractAdapter)

#include "tst_test_ormabstractadapter.moc"
