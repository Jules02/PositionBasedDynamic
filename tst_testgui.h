#ifndef TST_TESTGUI_H
#define TST_TESTGUI_H

#include <QObject>
#include <QtTest>

class MyTestGUI : public QObject {
    Q_OBJECT

private slots:
    void testCase1();
    void testCase2();
};


#endif // TST_TESTGUI_H