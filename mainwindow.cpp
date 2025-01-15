#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QWidget* simulation = new QWidget();
    simulation->setCursor(Qt::CrossCursor);
    this->draw_area = new DrawArea(800, 500, simulation);
    ui->verticalLayout->addWidget(simulation);

    auto timer = new QTimer();
    QObject::connect(timer, &QTimer::timeout, draw_area, &DrawArea::animate);
    timer->start(1);

    QObject::connect(ui->start_button, &QPushButton::clicked, this, &MainWindow::SwitchGravity);

    QObject::connect(ui->plan_colliders, &QPushButton::clicked, this, &MainWindow::AddingPlanColliders);

    QObject::connect(ui->sphere_colliders, &QPushButton::clicked, this, &MainWindow::AddingSphereColliders);


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::SwitchGravity()
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


void MainWindow::AddingSphereColliders()
{
    if (this->draw_area->context.addSphereColliderOn) {
        this->draw_area->context.addSphereColliderOn = false;
        ui->sphere_colliders->setText("Add Sphere Colliders");
    } else {
        this->draw_area->context.addSphereColliderOn = true;
        ui->sphere_colliders->setText("Stop Adding Sphere Colliders");
    }
}
