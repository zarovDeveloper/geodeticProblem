#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //changed main central widget
    ui->stackedWidget->setCurrentIndex(0);
    this->setCentralWidget(ui->stackedWidget);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_directGD_clicked()
{
    ui->stackedWidget->setCurrentIndex(1); //go to directGD
}

void MainWindow::on_pushButton_directGD_4_clicked()
{
    ui->stackedWidget->setCurrentIndex(0); //back to main scene
}
