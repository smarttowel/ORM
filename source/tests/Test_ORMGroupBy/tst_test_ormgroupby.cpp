#include <QString>
#include <QtTest>
#include "ormgroupby.cpp"

class Test_ORMGroupBy : public QObject
{
    Q_OBJECT
    
public:
    Test_ORMGroupBy();
    
private Q_SLOTS:
    void constructor();
    void operatorAND();
    void operatorEquals();
};

Test_ORMGroupBy::Test_ORMGroupBy()
{
}

void Test_ORMGroupBy::constructor()
{
    ORMGroupBy object;
    QCOMPARE(object.getGroupString().isEmpty(), true);
    ORMGroupBy object2("someField");
    QCOMPARE(object2.getGroupString(), QString("GROUP BY someField"));
}

void Test_ORMGroupBy::operatorAND()
{
    ORMGroupBy group1("field1"), group2("field2"), group3("field3");
    QCOMPARE(group1.getGroupString(), QString("GROUP BY field1"));
    group1 = ORMGroupBy("field1");
    QCOMPARE((group1 && group2).getGroupString(), QString("GROUP BY field1, field2"));
    group1 = ORMGroupBy("field1");
    QCOMPARE(((group1 && group2) && group3).getGroupString(), QString("GROUP BY field1, field2, field3"));
}

void Test_ORMGroupBy::operatorEquals()
{
    ORMGroupBy group1("field"), group2, group3;
    group2 = group1;
    QCOMPARE(group2.getGroupString(), QString("GROUP BY field"));
    group1 = group3;
    QCOMPARE(group1.getGroupString(), QString(""));
}

QTEST_APPLESS_MAIN(Test_ORMGroupBy)

#include "tst_test_ormgroupby.moc"
