#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QWidget* fractalDrawing = new QWidget();
    this->draw_area = new DrawArea(800, 300, fractalDrawing);
    ui->verticalLayout->addWidget(fractalDrawing);

    auto timer = new QTimer();
    QObject::connect(timer, &QTimer::timeout, draw_area, &DrawArea::animate);
    timer->start(1);

    QObject::connect(ui->start_button, &QPushButton::clicked, this, &MainWindow::HelloWorld);

    QObject::connect(ui->plan_colliders, &QPushButton::clicked, this, &MainWindow::AddingPlanColliders);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::HelloWorld()
{

    if (this->draw_area->context.isGravityOn) {
        this->draw_area->context.isGravityOn = false;
        ui->start_button->setText("Activate Gravity");
    } else {
        this->draw_area->context.isGravityOn = true;
        ui->start_button->setText("Deactivate Gravity");
    }

}

void MainWindow::AddingPlanColliders()
{
    if (this->draw_area->context.addPlanCollidersOn) {
        this->draw_area->context.addPlanCollidersOn = false;
        ui->plan_colliders->setText("Add Plan Colliders");
    } else {
        this->draw_area->context.addPlanCollidersOn = true;
        ui->plan_colliders->setText("Stop Adding Plan Colliders");
    }
}
