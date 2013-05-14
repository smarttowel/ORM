#include <QString>
#include <QtTest>
#include "parser.cpp"

class Test_ormtool : public QObject
{
    Q_OBJECT
    
public:
    Test_ormtool();
    
private Q_SLOTS:
    void removeTrash();
};

Test_ormtool::Test_ormtool()
{
}

void Test_ormtool::removeTrash()
{
    Parser parser;
    QCOMPARE(parser.removeTrash("abc\"def\"ghi"), QString("abcghi"));
    QCOMPARE(parser.removeTrash("abc\"def\\\"ghi"), QString("abc\"def\\\"ghi"));
    QCOMPARE(parser.removeTrash("abc\"d\"ef\"ghi"), QString("abcghi"));
    QCOMPARE(parser.removeTrash("abc\"d\\\"ef\"ghi"), QString("abcghi"));
    QCOMPARE(parser.removeTrash("abc//def"), QString("abc"));
    QCOMPARE(parser.removeTrash("abc//def*/ghi"), QString("abc*/ghi"));
    QCOMPARE(parser.removeTrash("abc\"def\"ghi//klm*/nop//rst"), QString("abcghi*/nop"));
}

QTEST_APPLESS_MAIN(Test_ormtool)

#include "tst_test_ormtool.moc"
