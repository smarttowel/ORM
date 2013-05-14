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
    void simplified();
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

void Test_ormtool::simplified()
{
    Parser parser;
    QString str = "abc/*def*/ghi";
    QCOMPARE(parser.simplified(str), QString("abcghi"));
    str = "abc/*de\nf*/ghi";
    QCOMPARE(parser.simplified(str), QString("abcghi"));
    str = "/*def*/abcghi";
    QCOMPARE(parser.simplified(str), QString("abcghi"));
    str = "abcghi/*def*/";
    QCOMPARE(parser.simplified(str), QString("abcghi"));
    str = "abcghi";
    QCOMPARE(parser.simplified(str), QString("abcghi"));
    str = "a\nb\n  c\t\rghi ";
    QCOMPARE(parser.simplified(str), QString("a b c ghi"));
    str = "a\nb\n  c\t\rg/*hi*/ ";
    QCOMPARE(parser.simplified(str), QString("a b c g"));
}

QTEST_APPLESS_MAIN(Test_ormtool)

#include "tst_test_ormtool.moc"
