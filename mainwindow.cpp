#include "mainwindow.h"
#include "ui_mainwindow.h"

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //changed main central widget
    ui->stackedWidget->setCurrentIndex(0);
    this->setCentralWidget(ui->stackedWidget);

    //set Validator in lineEdit

    QDoubleValidator *validatorDouble = new QDoubleValidator(0.0, qInf(), 2, this);
    ui->lineEdit_directGD_inputAx->setValidator(validatorDouble);
    ui->lineEdit_directGD_inputAy->setValidator(validatorDouble);
    ui->lineEdit_directGD_L->setValidator(validatorDouble);


    QIntValidator *validatorDegree = new QIntValidator(-360, 360, this);
    ui->lineEdit_directGD_angle_degrees->setValidator(validatorDegree);

    QIntValidator *validatorMinutes = new QIntValidator(0, 60, this);
    ui->lineEdit_directGD_angle_minutes->setValidator(validatorMinutes);

    QIntValidator *validatorSeconds = new QIntValidator(0, 3600, this);
    ui->lineEdit_directGD_angle_seconds->setValidator(validatorSeconds);
}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_pushButton_directGD_main_clicked() //button "Прямая геодезическая задача"
{
    ui->stackedWidget->setCurrentIndex(1); //go to directGD
}

void MainWindow::on_pushButton_directGD_back_clicked() //button "Назад" in directGD
{
    go_main();
}

void MainWindow::on_pushButton_directGD_solve_clicked() //button "Решить" in directGD
{
    directGD_solve();

    ui->stackedWidget->setCurrentIndex(2); //go to res directGD
}

void MainWindow::on_pushButton_directGDres_goToMain_clicked() //button "На главную страницу" in directGDres
{
    go_main();
}

void MainWindow::on_pushButton_directGDres_back_clicked() //button "Назад" in directGDres
{
    go_back();
}


//function

void MainWindow::go_main() //func to go to main page
{
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::go_back() //func to go to
{
    ui->stackedWidget->setCurrentIndex(ui->stackedWidget->currentIndex() - 1);
}

void MainWindow::directGD_solve()
{
    double latB, lonB; //coordinates of point B (x, y)
    double latA = ui->lineEdit_directGD_inputAx->text().toDouble(); //coordinates of point A (x)
    double lonA = ui->lineEdit_directGD_inputAy->text().toDouble(); //coordinates of point A (y)

    double degrees = ui->lineEdit_directGD_angle_degrees->text().toDouble(); //angle degrees
    double minutes = ui->lineEdit_directGD_angle_minutes->text().toDouble(); //angle minutes
    double seconds = ui->lineEdit_directGD_angle_seconds->text().toDouble(); //angle seconds
    double angle;

    double distance = ui->lineEdit_directGD_L->text().toInt();

    //solve angle

    angleOfDMS(degrees, minutes, seconds, angle); //angle of degrees. minutes, seconds

    directGD(latA, lonA, angle, distance, latB, lonB); //solve directGD

    ui->lineEdit_directGD_ansBx->setText(QString::number(latB));
    ui->lineEdit_directGD_ansBy->setText(QString::number(lonB));
}


