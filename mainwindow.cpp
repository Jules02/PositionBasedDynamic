#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QWidget* simulation = new QWidget();
    this->draw_area = new DrawArea(800, 300, simulation);
    ui->verticalLayout->addWidget(simulation);

    auto timer = new QTimer();
    QObject::connect(timer, &QTimer::timeout, draw_area, &DrawArea::animate);
    timer->start(1);


    QObject::connect(ui->start_button, &QPushButton::clicked, this, &MainWindow::SwitchGravity);

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
