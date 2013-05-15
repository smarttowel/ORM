#include <QString>
#include <QtTest>
#include <QList>
#include "model.cpp"
#include "property.cpp"
#include "parser.cpp"

class Test_ormtool : public QObject
{
    Q_OBJECT
    
public:
    Test_ormtool();
    
private Q_SLOTS:
    void removeTrash();
    void simplified();
    void cutModelInfo();
    void getModelFromString();
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

void Test_ormtool::cutModelInfo()
{
    Parser parser;
    QString str = "abcORMObject<Class1>abcORMObject<Class2>gfi";
    QList<QString> list = parser.cutModelInfo(str);
    QCOMPARE(list.value(0), QString("ORMObject<Class1>abc"));
    QCOMPARE(list.value(1), QString("ORMObject<Class2>gfi"));
    str = "ORMObject<Class1>abcORMObject<Class2>";
    list = parser.cutModelInfo(str);
    QCOMPARE(list.value(0), QString("ORMObject<Class1>abc"));
    QCOMPARE(list.value(1), QString("ORMObject<Class2>"));
    str = "abcgfi";
    list = parser.cutModelInfo(str);
    QCOMPARE(list.isEmpty(), true);
}

void Test_ormtool::getModelFromString()
{
    QString str = "ORMObject<Class1>ORM_PROPERTY(QString, name) ORM_PROPERTY(int,id)ORM_HAS_ONE(Class2)ORM_HAS_MANY(Class3)";
    Parser parser;
    Model model = parser.getModelFromString(str);
    QCOMPARE(model.name(), QString("Class1"));
    QCOMPARE(model.properties().value(0).getType(), QString("QString"));
    QCOMPARE(model.properties().value(0).getName(), QString("name"));
    QCOMPARE(model.properties().value(1).getType(), QString("int"));
    QCOMPARE(model.properties().value(1).getName(), QString("id"));
    QCOMPARE(model.properties().size(), 2);
    QCOMPARE(model.hasOne().first(), QString("Class2"));
    QCOMPARE(model.hasOne().size(), 1);
    QCOMPARE(model.hasMany().first(), QString("Class3"));
    QCOMPARE(model.hasMany().size(), 1);
    str = "ORMObject<Class0>sdfdkgrlyORM_PROPERTY(QString, name)fsdglORM_PROPERTY(int,id)sdfsdORM_HAS_ONE(Class2)sORM_HAS_ONE(Class3)fdf;"
            "ORM_HAS_MANY(Class4)sdfsdfORM_HAS_MANY(Class5)";
    model = parser.getModelFromString(str);
    QCOMPARE(model.name(), QString("Class0"));
    QCOMPARE(model.properties().value(0).getType(), QString("QString"));
    QCOMPARE(model.properties().value(0).getName(), QString("name"));
    QCOMPARE(model.properties().value(1).getType(), QString("int"));
    QCOMPARE(model.properties().value(1).getName(), QString("id"));
    QCOMPARE(model.properties().size(), 2);
    QCOMPARE(model.hasOne().first(), QString("Class2"));
    QCOMPARE(model.hasOne().value(1), QString("Class3"));
    QCOMPARE(model.hasOne().size(), 2);
    QCOMPARE(model.hasMany().first(), QString("Class4"));
    QCOMPARE(model.hasMany().value(1), QString("Class5"));
    QCOMPARE(model.hasMany().size(), 2);
}

QTEST_APPLESS_MAIN(Test_ormtool)

#include "tst_test_ormtool.moc"
