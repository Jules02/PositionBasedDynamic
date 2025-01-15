#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include "drawarea.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void SwitchGravity();
    void AddingPlanColliders();
    void AddingSphereColliders();
private:
    Ui::MainWindow *ui;
    DrawArea *draw_area;
};
#endif // MAINWINDOW_H
