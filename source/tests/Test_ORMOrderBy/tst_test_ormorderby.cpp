#include <QString>
#include <QtTest>
#include "ormorderby.cpp"

class Test_ORMOrderBy : public QObject
{
    Q_OBJECT
    
public:
    Test_ORMOrderBy();
    
private Q_SLOTS:
    void constructor();
    void operatorAND();
    void operatorEquals();
};

Test_ORMOrderBy::Test_ORMOrderBy()
{
}

void Test_ORMOrderBy::constructor()
{
    ORMOrderBy order, order2("field", ORMOrderBy::Ascending), order3("field", ORMOrderBy::Descending);
    QCOMPARE(order.getOrderString(), QString(""));
    QCOMPARE(order2.getOrderString(), QString("ORDER BY field ASC"));
    QCOMPARE(order3.getOrderString(), QString("ORDER BY field DESC"));
}

void Test_ORMOrderBy::operatorAND()
{
    ORMOrderBy order1("field1", ORMOrderBy::Descending), order2("field2", ORMOrderBy::Ascending), order3("field3", ORMOrderBy::Descending);
    QCOMPARE((order1 && order2).getOrderString(), QString("ORDER BY field1 DESC, field2 ASC"));
    QCOMPARE((order1 && order2 && order3).getOrderString(), QString("ORDER BY field1 DESC, field2 ASC, field3 DESC"));
    QCOMPARE(((order1 && order2) && order3).getOrderString(), QString("ORDER BY field1 DESC, field2 ASC, field3 DESC"));
}

void Test_ORMOrderBy::operatorEquals()
{
    ORMOrderBy order, order2("field", ORMOrderBy::Ascending), order3("field", ORMOrderBy::Descending);
    order2 = order3;
    QCOMPARE(order2.getOrderString(), QString("ORDER BY field DESC"));
    order2 = order;
    QCOMPARE(order.getOrderString(), QString(""));
}

QTEST_APPLESS_MAIN(Test_ORMOrderBy)

#include "tst_test_ormorderby.moc"
