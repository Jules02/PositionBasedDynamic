#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QWidget* fractalDrawing = new QWidget();
    this->draw_area = new DrawArea(fractalDrawing);
    this->draw_area->setFixedSize(800, 300);
    ui->verticalLayout->addWidget(fractalDrawing);

    auto timer = new QTimer();
    QObject::connect(timer, &QTimer::timeout, draw_area, &DrawArea::animate);
    timer->start(50);


    QObject::connect(ui->start_button, &QPushButton::clicked, this, &MainWindow::HelloWorld);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::HelloWorld()
{

    if (this->draw_area->isGravityOn) {
        this->draw_area->isGravityOn = false;
        ui->start_button->setText("Activate Gravity");
    } else {
        this->draw_area->isGravityOn = true;
        ui->start_button->setText("Deactivate Gravity");
    }

}
