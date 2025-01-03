#include "tst_testgui.h"

void MyTestGUI::testCase1() {
    int value = 42;
    QCOMPARE(value, 42);
}

void MyTestGUI::testCase2() {
    QString str = "QtTest";
    QVERIFY(str.contains("blabla"));
}

QTEST_MAIN(MyTestGUI)
#include "tst_testgui.moc"
