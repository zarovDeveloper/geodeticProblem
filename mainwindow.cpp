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


//page "Main"

void MainWindow::on_pushButton_directGD_main_clicked() //button "Прямая геодезическая задача" in main
{
    ui->stackedWidget->setCurrentIndex(1); //go to directGD
}

void MainWindow::on_pushButton_inverseGD_main_clicked() //button "Обратная геодезическая задача" in main
{
    ui->stackedWidget->setCurrentIndex(3); //go to reverseGD
}

//page "directGD

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
        ui->lineEdit_resDirectGD_Bx->setText(QString::number(latB, 'f', 2));
        ui->lineEdit_resDirectGD_By->setText(QString::number(lonB, 'f', 2));

        go_next();
    }
}

//page "directGDres"

void MainWindow::on_pushButton_resDirectGD_goToMain_clicked() //button "На главную страницу" in resDirectGD
{
    go_main();
}

void MainWindow::on_pushButton_resDirectGD_back_clicked() //button "Назад" in resDirectGD
{
    go_back();
}

//page "reverseGD"

void MainWindow::on_pushButton_reverseGD_back_clicked() //button "Назад" in reverseGD
{
    go_main();
}

void MainWindow::on_pushButton_reverseGD_solve_clicked() //button "Решить" in reverseGD
{
    double distance, degrees, minutes, seconds;
    double latA = ui->lineEdit_reversGD_inputAx->text().toDouble(); //coordinates of point A (x)
    double lonA = ui->lineEdit_reversGD_inputAy->text().toDouble(); //coordinates of point A (y)
    double latB = ui->lineEdit_reversGD_inputBx->text().toDouble(); //coordinates of point A (x)
    double lonB = ui->lineEdit_reversGD_inputBy->text().toDouble(); //coordinates of point A (y)

    reverseGD_solve(latA, lonA, latB, lonB, distance, degrees, minutes, seconds);

    ui->lineEdit_resReverseGD_outputL->setText(QString::number(distance, 'f', 2));
    ui->spinBox_resReverseGD_Angle_degrees->setValue(int(degrees));
    ui->spinBox_resReverseGD_Angle_minutes->setValue(int(minutes));
    ui->spinBox_resReverseGD_Angle_seconds->setValue(int(seconds));

    go_next();
}

//page "resReversGD"

void MainWindow::on_pushButton_resReverseGD_back_clicked() //button "Назад" in resReverseGD
{
    go_back();
}

void MainWindow::on_pushButton_resReverseGD_goToMain_clicked() //button "На главную страницу" in resReverseGD
{
    go_main();
}


//function


void MainWindow::go_main() //func to go to main page
{
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::go_back() //func to go back
{
    ui->stackedWidget->setCurrentIndex(ui->stackedWidget->currentIndex() - 1);
}

void MainWindow::go_next() //func to go next
{
    ui->stackedWidget->setCurrentIndex(ui->stackedWidget->currentIndex() + 1);
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

    //directGD
    ui->lineEdit_directGD_inputAx->setValidator(validatorDoubleMinus);
    ui->lineEdit_directGD_inputAy->setValidator(validatorDoubleMinus);
    //reverseGD
    ui->lineEdit_reversGD_inputAx->setValidator(validatorDoubleMinus);
    ui->lineEdit_reversGD_inputAy->setValidator(validatorDoubleMinus);
    ui->lineEdit_reversGD_inputBx->setValidator(validatorDoubleMinus);
    ui->lineEdit_reversGD_inputBy->setValidator(validatorDoubleMinus);

    ui->spinBox_resReverseGD_Angle_degrees->setButtonSymbols(QAbstractSpinBox::NoButtons);
    ui->spinBox_resReverseGD_Angle_minutes->setButtonSymbols(QAbstractSpinBox::NoButtons);
    ui->spinBox_resReverseGD_Angle_seconds->setButtonSymbols(QAbstractSpinBox::NoButtons);

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

//dynamic replace "," on "."

void MainWindow::on_lineEdit_reversGD_inputAx_textChanged(const QString &arg1)
{
    if (arg1.contains(","))
    {
        QString str = arg1;
        ui->lineEdit_reversGD_inputAx->setText(str.replace(",", "."));
    }
}

void MainWindow::on_lineEdit_reversGD_inputAy_textChanged(const QString &arg1)
{
    if (arg1.contains(","))
    {
        QString str = arg1;
        ui->lineEdit_reversGD_inputAy->setText(str.replace(",", "."));
    }
}

void MainWindow::on_lineEdit_reversGD_inputBx_textChanged(const QString &arg1)
{
    if (arg1.contains(","))
    {
        QString str = arg1;
        ui->lineEdit_reversGD_inputBx->setText(str.replace(",", "."));
    }
}

void MainWindow::on_lineEdit_reversGD_inputBy_textChanged(const QString &arg1)
{
    if (arg1.contains(","))
    {
        QString str = arg1;
        ui->lineEdit_reversGD_inputBy->setText(str.replace(",", "."));
    }
}

void MainWindow::on_lineEdit_directGD_inputAx_textChanged(const QString &arg1)
{
    if (arg1.contains(","))
    {
        QString str = arg1;
        ui->lineEdit_directGD_inputAx->setText(str.replace(",", "."));
    }
}

void MainWindow::on_lineEdit_directGD_inputAy_textChanged(const QString &arg1)
{
    if (arg1.contains(","))
    {
        QString str = arg1;
        ui->lineEdit_directGD_inputAy->setText(str.replace(",", "."));
    }
}

void MainWindow::on_lineEdit_directGD_inputL_textChanged(const QString &arg1)
{
    if (arg1.contains(","))
    {
        QString str = arg1;
        ui->lineEdit_directGD_inputL->setText(str.replace(",", "."));
    }
}
