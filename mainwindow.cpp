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

    //set Validator in lineEdit and spinBox
    setValidator();
}


MainWindow::~MainWindow()
{
    delete ui;
}


//button


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
    //find the variables
    double latB, lonB; //coordinates of point B (x, y)
    double latA = ui->lineEdit_directGD_inputAx->text().toDouble(); //coordinates of point A (x)
    double lonA = ui->lineEdit_directGD_inputAy->text().toDouble(); //coordinates of point A (y)

    double degrees = ui->spinBox_directGD_inputAngle_degrees->text().toDouble(); //angle degrees
    double minutes = ui->spinBox_directGD_inputAngle_minutes->text().toDouble(); //angle minutes
    double seconds = ui->spinBox_directGD_inputAngle_seconds->text().toDouble(); //angle seconds

    double distance = ui->lineEdit_directGD_inputL->text().toDouble(); //distance in sm
    int scale = ui->lineEdit_directGD_inputScale->text().toInt(); //scale


    if (QString::number(scale).right(2) != "00")
    {
        int scaleRound = 0;

        scaleRound = (scale / 100 + (scale % 100 >= 50 ? 1 : 0)) * 100;

        if (scaleRound == 0) scaleRound = 100; //round scale

        //Error
        QMessageBox msgBox;
        msgBox.setWindowTitle("Блокнот");
        msgBox.setText("Масштаб неккоректный. Хотите округлить до: " + QString::number(scaleRound) + "?");
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msgBox.setDefaultButton(QMessageBox::Yes);
        int res = msgBox.exec();
        switch (res)
        {//какую кнопку нажал юзер
        case QMessageBox::Yes:
        {
            ui->lineEdit_directGD_inputScale->setText(QString::number(scaleRound)); //change scale
            on_pushButton_directGD_solve_clicked();
            break;
        }
        case QMessageBox::No:
        {
            break;
        }
        }
    }
    else
    {
        //solve directGD
        directGD_solve(latA, lonA, degrees, minutes, seconds, distance, scale, latB, lonB);

        //rounding to 2 digits after
        ui->lineEdit_directGD_ansBx->setText(QString::number(latB, 'f', 2));
        ui->lineEdit_directGD_ansBy->setText(QString::number(lonB, 'f', 2));

        ui->stackedWidget->setCurrentIndex(2); //go to res directGD
    }
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


//changes


void MainWindow::setValidator() //change validator in lineEdit
{
    QDoubleValidator *validatorDouble = new QDoubleValidator(0.0, qInf(), 2, this);
    validatorDouble->setLocale(QLocale::English); //separator - point
    ui->lineEdit_directGD_inputL->setValidator(validatorDouble);
    ui->lineEdit_directGD_inputScale->setValidator(validatorDouble);

    QDoubleValidator *validatorDoubleMinus = new QDoubleValidator(-qInf(), qInf(), 2, this);
    validatorDoubleMinus->setLocale(QLocale::English); //separator - point
    ui->lineEdit_directGD_inputAx->setValidator(validatorDoubleMinus);
    ui->lineEdit_directGD_inputAy->setValidator(validatorDoubleMinus);

    ui->spinBox_directGD_inputAngle_degrees->setButtonSymbols(QAbstractSpinBox::NoButtons);
    ui->spinBox_directGD_inputAngle_degrees->setRange(0, 360); //the range of acceptable values

    ui->spinBox_directGD_inputAngle_minutes->setButtonSymbols(QAbstractSpinBox::NoButtons);
    ui->spinBox_directGD_inputAngle_minutes->setRange(0, 60); //the range of acceptable values

    ui->spinBox_directGD_inputAngle_seconds->setButtonSymbols(QAbstractSpinBox::NoButtons);
    ui->spinBox_directGD_inputAngle_seconds->setRange(0, 60); //the range of acceptable values
}

void MainWindow::on_lineEdit_directGD_inputScale_textChanged(const QString &arg1) //dynamic zoom change
{
    if (arg1.right(2) != "00")
        ui->lineEdit_directGD_inputScale->setStyleSheet(QString("font-size: %1px; color: red").arg(30));
    else
        ui->lineEdit_directGD_inputScale->setStyleSheet(QString("font-size: %1px; color: black").arg(30));
}
