#include <QString>
#include <QtTest>
#include "ormgroupby.cpp"

class Test_ORMGroupByTest : public QObject
{
    Q_OBJECT
    
public:
    Test_ORMGroupByTest();
    
private Q_SLOTS:
    void constructor();
};

Test_ORMGroupByTest::Test_ORMGroupByTest()
{
}

void Test_ORMGroupByTest::constructor()
{
    ORMGroupBy object;
    QCOMPARE(object.getGroupString().isEmpty(), true);
    ORMGroupBy object2("someField");
    QCOMPARE(object2.getGroupString(), QString("GROUP BY someField"));
}

QTEST_APPLESS_MAIN(Test_ORMGroupByTest)

#include "tst_test_ormgroupbytest.moc"
