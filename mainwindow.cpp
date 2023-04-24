#include "mainwindow.h"
#include "ui_mainwindow.h"

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) //constructor
{
    ui->setupUi(this);

    //changed main central widget
    ui->stackedWidget->setCurrentIndex(0);
    this->setCentralWidget(ui->stackedWidget);

    //set Validator in lineEdit and spinBox
    settings();

    //set radioButton
    ui->radioButton_directGD_inputL->click();
    ui->label_directGD_m_in_L->setVisible(false);
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
    double angle;

    double latA = ui->lineEdit_directGD_inputAx->text().toDouble(); //coordinates of point A (x)
    double lonA = ui->lineEdit_directGD_inputAy->text().toDouble(); //coordinates of point A (y)

    double degrees = ui->spinBox_directGD_inputAngle_degrees->text().toDouble(); //angle degrees
    double minutes = ui->spinBox_directGD_inputAngle_minutes->text().toDouble(); //angle minutes
    double seconds = ui->spinBox_directGD_inputAngle_seconds->text().toDouble(); //angle seconds

    double distance = ui->lineEdit_directGD_inputL->text().toDouble(); //distance in sm

    //check radioButton on scale/distance
    int scale;
    if (!ui->radioButton_directGD_inputHorizSpacing->isChecked())
    {//if need scale
        scale = ui->lineEdit_directGD_inputScale->text().toInt(); //scale
        distance = distance * scale / 100;
    }
    else
        scale = 100;


    switch (ui->comboBox_directGD->currentIndex())
    {//selected area
    case 0: //plane
    {
        //angle
        angleOfDMS(degrees, minutes, seconds, angle);

        //solve directGD
        directGDPlane(latA, lonA, angle, distance, latB, lonB);

        //rounding to 2 digits after
        ui->lineEdit_resDirectGD_Bx->setText(QString::number(latB, 'f', 2));
        ui->lineEdit_resDirectGD_By->setText(QString::number(lonB, 'f', 2));

        go_next();

        break;
    }
    case 1: //plane height
    {
        //new data
        double angleH, heightB;
        double height = ui->lineEdit_directGD_inputAh->text().toDouble(); //height point A

        //angle
        angleOfDMS(degrees, minutes, seconds, angle);

        degrees = ui->spinBox_directGD_inputAngle_degreesH->text().toDouble(); //angle degrees
        minutes = ui->spinBox_directGD_inputAngle_minutesH->text().toDouble(); //angle minutes
        seconds = ui->spinBox_directGD_inputAngle_secondsH->text().toDouble(); //angle seconds

        //angleH
        angleOfDMS(degrees, minutes, seconds, angleH);

        //solve directGDwithH
        directGDPlaneH(latA, lonA, height, angle, angleH, distance, latB, lonB, heightB);

        //rounding to 2 digits after
        ui->lineEdit_resDirectGD_Bx->setText(QString::number(latB, 'f', 2));
        ui->lineEdit_resDirectGD_By->setText(QString::number(lonB, 'f', 2));
        ui->lineEdit_resDirectGD_BH->setText(QString::number(heightB, 'f', 2));

        go_next();

        break;
    }
    case 2: //sphere
    {
        angleOfDMS(degrees, minutes, seconds, angle);

        degrees = ui->spinBox_directGD_inputX_degrees->value();
        minutes = ui->spinBox_directGD_inputX_minutes->value();
        seconds = ui->spinBox_directGD_inputX_seconds->value();

        angleOfDMS(degrees, minutes, seconds, latA);

        degrees = ui->spinBox_directGD_inputY_degrees->value();
        minutes = ui->spinBox_directGD_inputY_minutes->value();
        seconds = ui->spinBox_directGD_inputY_seconds->value();

        angleOfDMS(degrees, minutes, seconds, lonA);

        //solve directGDSphera
        directGDSphera(latA, lonA, angle, distance, latB, lonB);

        //output val
        angleFromDecimalAngle(latB, degrees, minutes, seconds);
        ui->spinBox_resDirectGD_BX_degrees->setValue(int(degrees));
        ui->spinBox_resDirectGD_BX_minutes->setValue(int(minutes));
        ui->spinBox_resDirectGD_BX_seconds->setValue(int(seconds));

        angleFromDecimalAngle(lonB, degrees, minutes, seconds);
        ui->spinBox_resDirectGD_BY_degrees->setValue(int(degrees));
        ui->spinBox_resDirectGD_BY_minutes->setValue(int(minutes));
        ui->spinBox_resDirectGD_BY_seconds->setValue(int(seconds));

        go_next();
        break;
    }
    case 3: //geod WGS-84
    {
        double angleRevers;

        angleOfDMS(degrees, minutes, seconds, angle);

        degrees = ui->spinBox_directGD_inputX_degrees->value();
        minutes = ui->spinBox_directGD_inputX_minutes->value();
        seconds = ui->spinBox_directGD_inputX_seconds->value();

        angleOfDMS(degrees, minutes, seconds, latA);

        degrees = ui->spinBox_directGD_inputY_degrees->value();
        minutes = ui->spinBox_directGD_inputY_minutes->value();
        seconds = ui->spinBox_directGD_inputY_seconds->value();

        angleOfDMS(degrees, minutes, seconds, lonA);

        //solve direct on geod
        directGDGeod(latA, lonA, angle, distance, 6356863.019, 0.0066934216, 0.0067385254, latB, lonB, angleRevers);

        //output val
        angleFromDecimalAngle(latB, degrees, minutes, seconds);
        ui->spinBox_resDirectGD_BX_degrees->setValue(int(degrees));
        ui->spinBox_resDirectGD_BX_minutes->setValue(int(minutes));
        ui->spinBox_resDirectGD_BX_seconds->setValue(int(seconds));

        angleFromDecimalAngle(lonB, degrees, minutes, seconds);
        ui->spinBox_resDirectGD_BY_degrees->setValue(int(degrees));
        ui->spinBox_resDirectGD_BY_minutes->setValue(int(minutes));
        ui->spinBox_resDirectGD_BY_seconds->setValue(int(seconds));

        go_next();
        break;
    }
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

//page "inverseGD"

void MainWindow::on_pushButton_inverseGD_back_clicked() //button "Назад" in reverseGD
{
    go_main();
}

void MainWindow::on_pushButton_inverseGD_solve_clicked() //button "Решить" in reverseGD
{
    //variable for data
    double distance, degrees, minutes, seconds, angle, lat[2], lon[2];

    lat[0] = ui->lineEdit_inverseGD_inputAx->text().toDouble(); //coordinates of point A (x)
    lon[0] = ui->lineEdit_inverseGD_inputAy->text().toDouble(); //coordinates of point A (y)
    lat[1] = ui->lineEdit_inverseGD_inputBx->text().toDouble(); //coordinates of point A (x)
    lon[1] = ui->lineEdit_inverseGD_inputBy->text().toDouble(); //coordinates of point A (y)

    switch (ui->comboBox_inverseGD->currentIndex())
    {//selected area
    case 0: //plane
    {
        //solve invresGD
        inversGDPlane(lat, lon, angle, distance);

        //from fraction in degrees, minutes, seconds
        angleFromDecimalAngle(angle, degrees, minutes, seconds);
        if (degrees < 0) degrees += 360;

        //output data
        ui->lineEdit_resInverseGD_outputL->setText(QString::number(distance, 'f', 2)); //distance output rounded to 2 decimal places
        ui->spinBox_resInverseGD_Angle_degrees->setValue(int(degrees));
        ui->spinBox_resInverseGD_Angle_minutes->setValue(int(minutes));
        ui->spinBox_resInverseGD_Angle_seconds->setValue(int(seconds));

        go_next();
        break;
    }
    case 1: //plane height
    {
        //new data height and angle between points
        double height[2], angleOrent;
        height[0] = ui->lineEdit_inverseGD_inputAH->text().toDouble(); //height of point A
        height[1] = ui->lineEdit_inverseGD_inputBH->text().toDouble(); //height of point B

        //solve inversGD
        inversGDPlaneH(lat, lon, height, angle, distance, angleOrent);

        //from fraction in degrees, minutes, seconds
        angleFromDecimalAngle(angle, degrees, minutes, seconds);
        if (degrees < 0) degrees += 360;

        ui->lineEdit_resInverseGD_outputL->setText(QString::number(distance, 'f', 2)); //distance output rounded to 2 decimal places
        //output directional angle
        ui->spinBox_resInverseGD_Angle_degrees->setValue(int(degrees));
        ui->spinBox_resInverseGD_Angle_minutes->setValue(int(minutes));
        ui->spinBox_resInverseGD_Angle_seconds->setValue(int(seconds));

        //from fraction in degrees, minutes, seconds
        angleFromDecimalAngle(angleOrent, degrees, minutes, seconds);
        //output angle between points
        ui->spinBox_resInverseGD_Angle_degreesH->setValue(int(degrees));
        ui->spinBox_resInverseGD_Angle_minutesH->setValue(int(minutes));
        ui->spinBox_resInverseGD_Angle_secondsH->setValue(int(seconds));

        go_next();
        break;
    }
    case 2: //sphera
    {
        degrees = ui->spinBox_invresGD_inputAX_degrees->value();
        minutes = ui->spinBox_invresGD_inputAX_minutes->value();
        seconds = ui->spinBox_invresGD_inputAX_seconds->value();
        angleOfDMS(degrees, minutes, seconds, lat[0]);

        degrees = ui->spinBox_invresGD_inputAY_degrees->value();
        minutes = ui->spinBox_invresGD_inputAY_minutes->value();
        seconds = ui->spinBox_invresGD_inputAY_seconds->value();
        angleOfDMS(degrees, minutes, seconds, lon[0]);

        degrees = ui->spinBox_invresGD_inputBX_degrees->value();
        minutes = ui->spinBox_invresGD_inputBX_minutes->value();
        seconds = ui->spinBox_invresGD_inputBX_seconds->value();
        angleOfDMS(degrees, minutes, seconds, lat[1]);

        degrees = ui->spinBox_invresGD_inputBY_degrees->value();
        minutes = ui->spinBox_invresGD_inputBY_minutes->value();
        seconds = ui->spinBox_invresGD_inputBY_seconds->value();
        angleOfDMS(degrees, minutes, seconds, lon[1]);

        inversGDSphera(lat, lon, angle, distance);

        angleFromDecimalAngle(angle, degrees, minutes, seconds);

        ui->lineEdit_resInverseGD_outputL->setText(QString::number(distance, 'f', 2)); //distance output rounded to 2 decimal places

        //output directional angle
        ui->spinBox_resInverseGD_Angle_degrees->setValue(int(degrees));
        ui->spinBox_resInverseGD_Angle_minutes->setValue(int(minutes));
        ui->spinBox_resInverseGD_Angle_seconds->setValue(int(seconds));

        go_next();
    }
    }
}

//page "resInverseGD"

void MainWindow::on_pushButton_resInverseGD_back_clicked() //button "Назад" in resReverseGD
{
    go_back();
}

void MainWindow::on_pushButton_resInverseGD_goToMain_clicked() //button "На главную страницу" in resReverseGD
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


void MainWindow::settings() //settings
{
    //validator for lineEdit with a range of acceptable values from 0 to inf
    QDoubleValidator *validatorDouble = new QDoubleValidator(0.0, qInf(), 2, this);
    validatorDouble->setLocale(QLocale::English); //separator - point


    //validator for lineEdit with a range of acceptable values from 0 to inf
    QDoubleValidator *validatorDoubleMinus = new QDoubleValidator(-qInf(), qInf(), 2, this);
    validatorDoubleMinus->setLocale(QLocale::English); //separator - point

    //directGD
    ui->lineEdit_directGD_inputL->setValidator(validatorDouble);
    ui->lineEdit_directGD_inputScale->setValidator(validatorDouble);

    ui->lineEdit_directGD_inputAx->setValidator(validatorDoubleMinus);
    ui->lineEdit_directGD_inputAy->setValidator(validatorDoubleMinus);
    ui->lineEdit_directGD_inputAh->setValidator(validatorDoubleMinus);

    ui->lineEdit_directGD_inputAh->setVisible(false);
    ui->label_directGD_inputH->setVisible(false);
    ui->label_directGD_m_h->setVisible(false);

    ui->spinBox_directGD_inputAngle_degrees->setButtonSymbols(QAbstractSpinBox::NoButtons);
    ui->spinBox_directGD_inputAngle_degrees->setRange(0, 360); //the range of acceptable values

    ui->spinBox_directGD_inputAngle_minutes->setButtonSymbols(QAbstractSpinBox::NoButtons);
    ui->spinBox_directGD_inputAngle_minutes->setRange(0, 60); //the range of acceptable values

    ui->spinBox_directGD_inputAngle_seconds->setButtonSymbols(QAbstractSpinBox::NoButtons);
    ui->spinBox_directGD_inputAngle_seconds->setRange(0, 60); //the range of acceptable values

    ui->spinBox_directGD_inputAngle_degreesH->setButtonSymbols(QAbstractSpinBox::NoButtons);
    ui->spinBox_directGD_inputAngle_degreesH->setRange(-90, 90); //the range of acceptable values

    ui->spinBox_directGD_inputAngle_minutesH->setButtonSymbols(QAbstractSpinBox::NoButtons);
    ui->spinBox_directGD_inputAngle_minutesH->setRange(0, 60); //the range of acceptable values

    ui->spinBox_directGD_inputAngle_secondsH->setButtonSymbols(QAbstractSpinBox::NoButtons);
    ui->spinBox_directGD_inputAngle_secondsH->setRange(0, 60); //the range of acceptable values

    ui->lineEdit_directGD_inputAx->setVisible(true);
    ui->spinBox_directGD_inputX_degrees->setVisible(false);
    ui->spinBox_directGD_inputX_degrees->setButtonSymbols(QAbstractSpinBox::NoButtons);
    ui->spinBox_directGD_inputX_degrees->setRange(0, 360);
    ui->spinBox_directGD_inputX_minutes->setVisible(false);
    ui->spinBox_directGD_inputX_minutes->setButtonSymbols(QAbstractSpinBox::NoButtons);
    ui->spinBox_directGD_inputX_minutes->setRange(0, 60);
    ui->spinBox_directGD_inputX_seconds->setVisible(false);
    ui->spinBox_directGD_inputX_seconds->setButtonSymbols(QAbstractSpinBox::NoButtons);
    ui->spinBox_directGD_inputX_seconds->setRange(0, 60);
    ui->label_directGD_x_degree->setVisible(false);
    ui->label_directGD_x_minutes->setVisible(false);
    ui->label_directGD_x_seconds->setVisible(false);

    ui->lineEdit_directGD_inputAy->setVisible(true);
    ui->spinBox_directGD_inputY_degrees->setVisible(false);
    ui->spinBox_directGD_inputY_degrees->setButtonSymbols(QAbstractSpinBox::NoButtons);
    ui->spinBox_directGD_inputY_degrees->setRange(0, 360);
    ui->spinBox_directGD_inputY_minutes->setVisible(false);
    ui->spinBox_directGD_inputY_minutes->setButtonSymbols(QAbstractSpinBox::NoButtons);
    ui->spinBox_directGD_inputX_minutes->setRange(0, 60);
    ui->spinBox_directGD_inputY_seconds->setVisible(false);
    ui->spinBox_directGD_inputY_seconds->setButtonSymbols(QAbstractSpinBox::NoButtons);
    ui->spinBox_directGD_inputY_seconds->setRange(0, 60);
    ui->label_directGD_y_degree->setVisible(false);
    ui->label_directGD_y_minutes->setVisible(false);
    ui->label_directGD_y_seconds->setVisible(false);

    ui->lineEdit_directGD_inputAh->setVisible(false);
    ui->label_directGD_inputH->setVisible(false);
    ui->label_directGD_m_h->setVisible(false);
    ui->label_directGD_AngleH->setVisible(false);
    ui->label_directGD_degree_H->setVisible(false);
    ui->label_directGD_minutes_H->setVisible(false);
    ui->label_directGD_seconds_H->setVisible(false);
    ui->label_directGD_inputAngle_H->setVisible(false);
    ui->spinBox_directGD_inputAngle_degreesH->setVisible(false);
    ui->spinBox_directGD_inputAngle_degreesH->setButtonSymbols(QAbstractSpinBox::NoButtons);
    ui->spinBox_directGD_inputAngle_minutesH->setVisible(false);
    ui->spinBox_directGD_inputAngle_minutesH->setButtonSymbols(QAbstractSpinBox::NoButtons);
    ui->spinBox_directGD_inputAngle_secondsH->setVisible(false);
    ui->spinBox_directGD_inputAngle_secondsH->setButtonSymbols(QAbstractSpinBox::NoButtons);

    ui->label_directGD_m_in_L->setText("м");
    ui->label_directGD_m_scale->setText("см    при масштабе      1:");
    ui->lineEdit_directGD_inputScale->setText(QString::number(100));

    //resDirectGD

    ui->label_resDirectGD_BH->setVisible(false);
    ui->label_resDirectGD_coordB_H->setVisible(false);
    ui->label_resDirectGD_m_H->setVisible(false);
    ui->lineEdit_resDirectGD_BH->setVisible(false);

    //inverseGD

    ui->lineEdit_inverseGD_inputAx->setVisible(true);
    ui->spinBox_invresGD_inputAX_degrees->setVisible(false);
    ui->spinBox_invresGD_inputAX_degrees->setButtonSymbols(QAbstractSpinBox::NoButtons);
    ui->spinBox_invresGD_inputAX_degrees->setRange(0, 360);
    ui->label_inversGD_Ax_degree->setVisible(false);
    ui->spinBox_invresGD_inputAX_minutes->setVisible(false);
    ui->spinBox_invresGD_inputAX_minutes->setButtonSymbols(QAbstractSpinBox::NoButtons);
    ui->spinBox_invresGD_inputAX_minutes->setRange(0, 60);
    ui->label_inversGD_Ax_minute->setVisible(false);
    ui->spinBox_invresGD_inputAX_seconds->setVisible(false);
    ui->spinBox_invresGD_inputAX_seconds->setButtonSymbols(QAbstractSpinBox::NoButtons);
    ui->spinBox_invresGD_inputAX_seconds->setRange(0, 60);
    ui->label_inversGD_Ax_second->setVisible(false);

    ui->lineEdit_inverseGD_inputAy->setVisible(true);
    ui->spinBox_invresGD_inputAY_degrees->setVisible(false);
    ui->spinBox_invresGD_inputAY_degrees->setButtonSymbols(QAbstractSpinBox::NoButtons);
    ui->spinBox_invresGD_inputAY_degrees->setRange(0, 360);
    ui->label_inversGD_Ay_degree->setVisible(false);
    ui->spinBox_invresGD_inputAY_minutes->setVisible(false);
    ui->spinBox_invresGD_inputAY_minutes->setButtonSymbols(QAbstractSpinBox::NoButtons);
    ui->spinBox_invresGD_inputAY_minutes->setRange(0, 60);
    ui->label_inversGD_Ay_minute->setVisible(false);
    ui->spinBox_invresGD_inputAY_seconds->setVisible(false);
    ui->spinBox_invresGD_inputAY_seconds->setButtonSymbols(QAbstractSpinBox::NoButtons);
    ui->spinBox_invresGD_inputAY_seconds->setRange(0, 60);
    ui->label_inversGD_Ay_second->setVisible(false);

    ui->lineEdit_inverseGD_inputBx->setVisible(true);
    ui->spinBox_invresGD_inputBX_degrees->setVisible(false);
    ui->spinBox_invresGD_inputBX_degrees->setButtonSymbols(QAbstractSpinBox::NoButtons);
    ui->spinBox_invresGD_inputBX_degrees->setRange(0, 360);
    ui->label_inversGD_Bx_degree->setVisible(false);
    ui->spinBox_invresGD_inputBX_minutes->setVisible(false);
    ui->spinBox_invresGD_inputBX_minutes->setButtonSymbols(QAbstractSpinBox::NoButtons);
    ui->spinBox_invresGD_inputBX_minutes->setRange(0, 60);
    ui->label_inversGD_Bx_minute->setVisible(false);
    ui->spinBox_invresGD_inputBX_seconds->setVisible(false);
    ui->spinBox_invresGD_inputBX_seconds->setButtonSymbols(QAbstractSpinBox::NoButtons);
    ui->spinBox_invresGD_inputBX_seconds->setRange(0, 60);
    ui->label_inversGD_Bx_second->setVisible(false);

    ui->lineEdit_inverseGD_inputBy->setVisible(true);
    ui->spinBox_invresGD_inputBY_degrees->setVisible(false);
    ui->spinBox_invresGD_inputBY_degrees->setButtonSymbols(QAbstractSpinBox::NoButtons);
    ui->spinBox_invresGD_inputBY_degrees->setRange(0, 360);
    ui->label_inversGD_By_degree->setVisible(false);
    ui->spinBox_invresGD_inputBY_minutes->setVisible(false);
    ui->spinBox_invresGD_inputBY_minutes->setButtonSymbols(QAbstractSpinBox::NoButtons);
    ui->spinBox_invresGD_inputBY_minutes->setRange(0, 60);
    ui->label_inversGD_By_minute->setVisible(false);
    ui->spinBox_invresGD_inputBY_seconds->setVisible(false);
    ui->spinBox_invresGD_inputBY_seconds->setButtonSymbols(QAbstractSpinBox::NoButtons);
    ui->spinBox_invresGD_inputBY_seconds->setRange(0, 60);
    ui->label_inversGD_By_second->setVisible(false);

    ui->lineEdit_inverseGD_inputAx->setValidator(validatorDoubleMinus);
    ui->lineEdit_inverseGD_inputAy->setValidator(validatorDoubleMinus);
    ui->lineEdit_inverseGD_inputBx->setValidator(validatorDoubleMinus);
    ui->lineEdit_inverseGD_inputBy->setValidator(validatorDoubleMinus);

    ui->label_inverseGD_m_h->setVisible(false);
    ui->label_inverseGD_m_h_2->setVisible(false);
    ui->label_inverseGD_inputAH->setVisible(false);
    ui->label_inverseGD_inputBH->setVisible(false);
    ui->lineEdit_inverseGD_inputAH->setVisible(false);
    ui->lineEdit_inverseGD_inputBH->setVisible(false);

    //resInverseGD

    ui->spinBox_resInverseGD_Angle_degrees->setButtonSymbols(QAbstractSpinBox::NoButtons);
    ui->spinBox_resInverseGD_Angle_minutes->setButtonSymbols(QAbstractSpinBox::NoButtons);
    ui->spinBox_resInverseGD_Angle_seconds->setButtonSymbols(QAbstractSpinBox::NoButtons);

    ui->spinBox_resInverseGD_Angle_degreesH->setButtonSymbols(QAbstractSpinBox::NoButtons);
    ui->spinBox_resInverseGD_Angle_degreesH->setRange(-90, 90);

    ui->spinBox_resInverseGD_Angle_minutesH->setButtonSymbols(QAbstractSpinBox::NoButtons);
    ui->spinBox_resInverseGD_Angle_minutesH->setRange(0, 60);

    ui->spinBox_resInverseGD_Angle_secondsH->setButtonSymbols(QAbstractSpinBox::NoButtons);
    ui->spinBox_resInverseGD_Angle_secondsH->setRange(0, 60);

    ui->label_resInverseGD_AngleH->setVisible(false);
    ui->label_resInverse_degreeH->setVisible(false);
    ui->label_resInverseGD_angleH->setVisible(false);
    ui->label_resInverseGD_minutesH->setVisible(false);
    ui->label_resInverseGD_secondsH->setVisible(false);
    ui->spinBox_resInverseGD_Angle_degreesH->setVisible(false);
    ui->spinBox_resInverseGD_Angle_minutesH->setVisible(false);
    ui->spinBox_resInverseGD_Angle_secondsH->setVisible(false);

    ui->lineEdit_resDirectGD_Bx->setVisible(true);
    ui->spinBox_resDirectGD_BX_degrees->setVisible(false);
    ui->spinBox_resDirectGD_BX_degrees->setButtonSymbols(QAbstractSpinBox::NoButtons);
    ui->label_resDirectGD_x_degree->setVisible(false);
    ui->spinBox_resDirectGD_BX_minutes->setVisible(false);
    ui->spinBox_resDirectGD_BX_minutes->setButtonSymbols(QAbstractSpinBox::NoButtons);
    ui->label_resDirectGD_x_minute->setVisible(false);
    ui->spinBox_resDirectGD_BX_seconds->setVisible(false);
    ui->spinBox_resDirectGD_BX_seconds->setButtonSymbols(QAbstractSpinBox::NoButtons);
    ui->label_resDirectGD_x_second->setVisible(false);

    ui->lineEdit_resDirectGD_By->setVisible(true);
    ui->spinBox_resDirectGD_BY_degrees->setVisible(false);
    ui->spinBox_resDirectGD_BY_degrees->setButtonSymbols(QAbstractSpinBox::NoButtons);
    ui->label_resDirectGD_y_degree->setVisible(false);
    ui->spinBox_resDirectGD_BY_minutes->setVisible(false);
    ui->spinBox_resDirectGD_BY_minutes->setButtonSymbols(QAbstractSpinBox::NoButtons);
    ui->label_resDirectGD_y_minute->setVisible(false);
    ui->spinBox_resDirectGD_BY_seconds->setVisible(false);
    ui->spinBox_resDirectGD_BY_seconds->setButtonSymbols(QAbstractSpinBox::NoButtons);
    ui->label_resDirectGD_y_second->setVisible(false);

}

void MainWindow::on_lineEdit_directGD_inputScale_textChanged(const QString &arg1) //dynamic scale change
{
    if ((arg1.right(2) != "00") or !(arg1.toDouble() > 0))
    {//if arg1 not ending on 00 or arg1 - 0
        ui->lineEdit_directGD_inputScale->setStyleSheet(QString("font-size: %1px; color: red").arg(28));
        ui->pushButton_directGD_solve->setEnabled(false);
    }
    else
    {//if all ok
        ui->lineEdit_directGD_inputScale->setStyleSheet(QString("font-size: %1px; color: black").arg(28));
        ui->pushButton_directGD_solve->setEnabled(true);
    }

    if(arg1.contains("e"))
    {
        QString str = arg1;
        str.remove("e");
        ui->lineEdit_directGD_inputScale->setText(str);
    }
}

//dynamic replace "," on "."

void MainWindow::on_lineEdit_inverseGD_inputAx_textChanged(const QString &arg1)
{
    if (arg1.contains(","))
    {
        QString str = arg1;
        ui->lineEdit_inverseGD_inputAx->setText(str.replace(",", "."));
    }
}

void MainWindow::on_lineEdit_inverseGD_inputAy_textChanged(const QString &arg1)
{
    if (arg1.contains(","))
    {
        QString str = arg1;
        ui->lineEdit_inverseGD_inputAy->setText(str.replace(",", "."));
    }
}

void MainWindow::on_lineEdit_inverseGD_inputBx_textChanged(const QString &arg1)
{
    if (arg1.contains(","))
    {
        QString str = arg1;
        ui->lineEdit_inverseGD_inputBx->setText(str.replace(",", "."));
    }
}

void MainWindow::on_lineEdit_inverseGD_inputBy_textChanged(const QString &arg1)
{
    if (arg1.contains(","))
    {
        QString str = arg1;
        ui->lineEdit_inverseGD_inputBy->setText(str.replace(",", "."));
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

//chenges angle

void MainWindow::on_spinBox_directGD_inputAngle_degrees_valueChanged(int arg1)
{
    if (arg1 == 360)
    {
        ui->spinBox_directGD_inputAngle_minutes->setValue(0);
        ui->spinBox_directGD_inputAngle_minutes->setRange(0, 0);

        ui->spinBox_directGD_inputAngle_seconds->setValue(0);
        ui->spinBox_directGD_inputAngle_seconds->setRange(0, 0);
    }
    else {
        ui->spinBox_directGD_inputAngle_minutes->setRange(0, 60);
        ui->spinBox_directGD_inputAngle_seconds->setRange(0, 60);
    }
}

void MainWindow::on_spinBox_directGD_inputAngle_minutes_valueChanged(int arg1)
{
    if (arg1 == 60)
    {
        ui->spinBox_directGD_inputAngle_degrees->setValue(ui->spinBox_directGD_inputAngle_degrees->value() + 1);
        ui->spinBox_directGD_inputAngle_minutes->setValue(0);
    }
}

void MainWindow::on_spinBox_directGD_inputAngle_seconds_valueChanged(int arg1)
{
    if (arg1 == 60)
    {
        ui->spinBox_directGD_inputAngle_minutes->setValue(ui->spinBox_directGD_inputAngle_minutes->value() + 1);
        ui->spinBox_directGD_inputAngle_seconds->setValue(0);
    }
}

void MainWindow::on_spinBox_directGD_inputAngle_degreesH_valueChanged(int arg1)
{
    if (arg1 == 360)
    {
        ui->spinBox_directGD_inputAngle_minutesH->setValue(0);
        ui->spinBox_directGD_inputAngle_minutesH->setRange(0, 0);

        ui->spinBox_directGD_inputAngle_secondsH->setValue(0);
        ui->spinBox_directGD_inputAngle_secondsH->setRange(0, 0);
    }
    else {
        ui->spinBox_directGD_inputAngle_minutesH->setRange(0, 60);
        ui->spinBox_directGD_inputAngle_secondsH->setRange(0, 60);
    }
}

void MainWindow::on_spinBox_directGD_inputAngle_minutesH_valueChanged(int arg1)
{
    if (arg1 == 60)
    {
        ui->spinBox_directGD_inputAngle_degreesH->setValue(ui->spinBox_directGD_inputAngle_degreesH->value() + 1);
        ui->spinBox_directGD_inputAngle_minutesH->setValue(0);
    }
}

void MainWindow::on_spinBox_directGD_inputAngle_secondsH_valueChanged(int arg1)
{
    if (arg1 == 60)
    {
        ui->spinBox_directGD_inputAngle_minutesH->setValue(ui->spinBox_directGD_inputAngle_minutesH->value() + 1);
        ui->spinBox_directGD_inputAngle_secondsH->setValue(0);
    }
}

void MainWindow::on_spinBox_resInverseGD_Angle_minutesH_valueChanged(int arg1)
{
    if (arg1 == 60)
    {
        ui->spinBox_resInverseGD_Angle_degreesH->setValue(ui->spinBox_resInverseGD_Angle_degreesH->value() + 1);
        ui->spinBox_resInverseGD_Angle_minutesH->setValue(0);
    }
}

void MainWindow::on_spinBox_resInverseGD_Angle_secondsH_valueChanged(int arg1)
{
    if (arg1 == 60)
    {
        ui->spinBox_resInverseGD_Angle_minutesH->setValue(ui->spinBox_resInverseGD_Angle_minutesH->value() + 1);
        ui->spinBox_resInverseGD_Angle_secondsH->setValue(0);
    }
}

void MainWindow::on_spinBox_directGD_inputX_degrees_valueChanged(int arg1)
{
    if (arg1 == 360)
    {
        ui->spinBox_directGD_inputX_minutes->setValue(0);
        ui->spinBox_directGD_inputX_minutes->setRange(0, 0);

        ui->spinBox_directGD_inputX_seconds->setValue(0);
        ui->spinBox_directGD_inputX_seconds->setRange(0, 0);
    }
    else {
        ui->spinBox_directGD_inputX_minutes->setRange(0, 60);
        ui->spinBox_directGD_inputX_seconds->setRange(0, 60);
    }
}

void MainWindow::on_spinBox_directGD_inputX_minutes_valueChanged(int arg1)
{
    if (arg1 == 60)
    {
        ui->spinBox_directGD_inputX_degrees->setValue(ui->spinBox_directGD_inputX_degrees->value() + 1);
        ui->spinBox_directGD_inputX_minutes->setValue(0);
    }
}

void MainWindow::on_spinBox_directGD_inputX_seconds_valueChanged(int arg1)
{
    if (arg1 == 60)
    {
        ui->spinBox_directGD_inputX_minutes->setValue(ui->spinBox_directGD_inputX_minutes->value() + 1);
        ui->spinBox_directGD_inputX_seconds->setValue(0);
    }
}

void MainWindow::on_spinBox_directGD_inputY_degrees_valueChanged(int arg1)
{
    if (arg1 == 360)
    {
        ui->spinBox_directGD_inputY_minutes->setValue(0);
        ui->spinBox_directGD_inputY_minutes->setRange(0, 0);

        ui->spinBox_directGD_inputY_seconds->setValue(0);
        ui->spinBox_directGD_inputY_seconds->setRange(0, 0);
    }
    else {
        ui->spinBox_directGD_inputY_minutes->setRange(0, 60);
        ui->spinBox_directGD_inputY_seconds->setRange(0, 60);
    }
}

void MainWindow::on_spinBox_directGD_inputY_minutes_valueChanged(int arg1)
{
    if (arg1 == 60)
    {
        ui->spinBox_directGD_inputY_degrees->setValue(ui->spinBox_directGD_inputY_degrees->value() + 1);
        ui->spinBox_directGD_inputY_minutes->setValue(0);
    }
}

void MainWindow::on_spinBox_directGD_inputY_seconds_valueChanged(int arg1)
{
    if (arg1 == 60)
    {
        ui->spinBox_directGD_inputY_minutes->setValue(ui->spinBox_directGD_inputY_minutes->value() + 1);
        ui->spinBox_directGD_inputY_seconds->setValue(0);
    }
}



//choice of option L


void MainWindow::on_radioButton_directGD_inputL_clicked()
{
    ui->label_directGD_m_scale->setVisible(true);
    ui->lineEdit_directGD_inputScale->setVisible(true);
    ui->label_directGD_m_in_L->setVisible(false);
}

void MainWindow::on_radioButton_directGD_inputHorizSpacing_clicked()
{
    ui->label_directGD_m_in_L->setVisible(true);
    ui->label_directGD_m_scale->setVisible(false);
    ui->lineEdit_directGD_inputScale->setVisible(false);
}


//comboBox on direct page


//on direct page

void MainWindow::on_comboBox_directGD_currentIndexChanged(int index)
{
    if (index == 0)
    {//choose "Плоскость"
        ui->label_directGD_m_in_L->setText("м");
        ui->label_directGD_m_scale->setText("см    при масштабе      1:");
        ui->lineEdit_directGD_inputScale->setText(QString::number(100));


        ui->lineEdit_directGD_inputAx->setVisible(true);
        ui->spinBox_directGD_inputX_degrees->setVisible(false);
        ui->spinBox_directGD_inputX_minutes->setVisible(false);
        ui->spinBox_directGD_inputX_seconds->setVisible(false);
        ui->label_directGD_x_degree->setVisible(false);
        ui->label_directGD_x_minutes->setVisible(false);
        ui->label_directGD_x_seconds->setVisible(false);

        ui->lineEdit_directGD_inputAy->setVisible(true);
        ui->spinBox_directGD_inputY_degrees->setVisible(false);
        ui->spinBox_directGD_inputY_minutes->setVisible(false);
        ui->spinBox_directGD_inputY_seconds->setVisible(false);
        ui->label_directGD_y_degree->setVisible(false);
        ui->label_directGD_y_minutes->setVisible(false);
        ui->label_directGD_y_seconds->setVisible(false);

        ui->lineEdit_resDirectGD_Bx->setVisible(true);
        ui->spinBox_resDirectGD_BX_degrees->setVisible(false);
        ui->label_resDirectGD_x_degree->setVisible(false);
        ui->spinBox_resDirectGD_BX_minutes->setVisible(false);
        ui->label_resDirectGD_x_minute->setVisible(false);
        ui->spinBox_resDirectGD_BX_seconds->setVisible(false);
        ui->label_resDirectGD_x_second->setVisible(false);

        ui->lineEdit_resDirectGD_By->setVisible(true);
        ui->spinBox_resDirectGD_BY_degrees->setVisible(false);
        ui->label_resDirectGD_y_degree->setVisible(false);
        ui->spinBox_resDirectGD_BY_minutes->setVisible(false);
        ui->label_resDirectGD_y_minute->setVisible(false);
        ui->spinBox_resDirectGD_BY_seconds->setVisible(false);
        ui->label_resDirectGD_y_second->setVisible(false);

        ui->lineEdit_directGD_inputAh->setVisible(false);
        ui->label_directGD_inputH->setVisible(false);
        ui->label_directGD_m_h->setVisible(false);
        ui->label_directGD_AngleH->setVisible(false);
        ui->label_directGD_degree_H->setVisible(false);
        ui->label_directGD_minutes_H->setVisible(false);
        ui->label_directGD_seconds_H->setVisible(false);
        ui->label_directGD_inputAngle_H->setVisible(false);
        ui->spinBox_directGD_inputAngle_degreesH->setVisible(false);
        ui->spinBox_directGD_inputAngle_minutesH->setVisible(false);
        ui->spinBox_directGD_inputAngle_secondsH->setVisible(false);
        ui->label_resDirectGD_BH->setVisible(false);
        ui->label_resDirectGD_coordB_H->setVisible(false);
        ui->label_resDirectGD_m_H->setVisible(false);
        ui->lineEdit_resDirectGD_BH->setVisible(false);
    }
    else if (index == 1)
    {//choose "Плоскость с высотой"
        ui->label_directGD_m_in_L->setText("м");
        ui->label_directGD_m_scale->setText("см    при масштабе      1:");
        ui->lineEdit_directGD_inputScale->setText(QString::number(100));

        ui->lineEdit_directGD_inputAx->setVisible(true);
        ui->spinBox_directGD_inputX_degrees->setVisible(false);
        ui->spinBox_directGD_inputX_minutes->setVisible(false);
        ui->spinBox_directGD_inputX_seconds->setVisible(false);
        ui->label_directGD_x_degree->setVisible(false);
        ui->label_directGD_x_minutes->setVisible(false);
        ui->label_directGD_x_seconds->setVisible(false);

        ui->lineEdit_directGD_inputAy->setVisible(true);
        ui->spinBox_directGD_inputY_degrees->setVisible(false);
        ui->spinBox_directGD_inputY_minutes->setVisible(false);
        ui->spinBox_directGD_inputY_seconds->setVisible(false);
        ui->label_directGD_y_degree->setVisible(false);
        ui->label_directGD_y_minutes->setVisible(false);
        ui->label_directGD_y_seconds->setVisible(false);

        ui->lineEdit_resDirectGD_Bx->setVisible(true);
        ui->spinBox_resDirectGD_BX_degrees->setVisible(false);
        ui->label_resDirectGD_x_degree->setVisible(false);
        ui->spinBox_resDirectGD_BX_minutes->setVisible(false);
        ui->label_resDirectGD_x_minute->setVisible(false);
        ui->spinBox_resDirectGD_BX_seconds->setVisible(false);
        ui->label_resDirectGD_x_second->setVisible(false);

        ui->lineEdit_resDirectGD_By->setVisible(true);
        ui->spinBox_resDirectGD_BY_degrees->setVisible(false);
        ui->label_resDirectGD_y_degree->setVisible(false);
        ui->spinBox_resDirectGD_BY_minutes->setVisible(false);
        ui->label_resDirectGD_y_minute->setVisible(false);
        ui->spinBox_resDirectGD_BY_seconds->setVisible(false);
        ui->label_resDirectGD_y_second->setVisible(false);

        ui->lineEdit_directGD_inputAh->setVisible(true);
        ui->label_directGD_inputH->setVisible(true);
        ui->label_directGD_m_h->setVisible(true);
        ui->label_directGD_AngleH->setVisible(true);
        ui->label_directGD_degree_H->setVisible(true);
        ui->label_directGD_minutes_H->setVisible(true);
        ui->label_directGD_seconds_H->setVisible(true);
        ui->label_directGD_inputAngle_H->setVisible(true);
        ui->spinBox_directGD_inputAngle_degreesH->setVisible(true);
        ui->spinBox_directGD_inputAngle_minutesH->setVisible(true);
        ui->spinBox_directGD_inputAngle_secondsH->setVisible(true);
        ui->label_resDirectGD_BH->setVisible(true);
        ui->label_resDirectGD_coordB_H->setVisible(true);
        ui->label_resDirectGD_m_H->setVisible(true);
        ui->lineEdit_resDirectGD_BH->setVisible(true);
    }
    else if (index == 2)
    {//choose "Сфера"
        ui->label_directGD_m_in_L->setText("км");
        ui->label_directGD_m_scale->setText("м     при масштабе       1:");
        ui->lineEdit_directGD_inputScale->setText(QString::number(1000));

        ui->lineEdit_directGD_inputAx->setVisible(false);
        ui->spinBox_directGD_inputX_degrees->setVisible(true);
        ui->spinBox_directGD_inputX_minutes->setVisible(true);
        ui->spinBox_directGD_inputX_seconds->setVisible(true);
        ui->label_directGD_x_degree->setVisible(true);
        ui->label_directGD_x_minutes->setVisible(true);
        ui->label_directGD_x_seconds->setVisible(true);

        ui->lineEdit_directGD_inputAy->setVisible(false);
        ui->spinBox_directGD_inputY_degrees->setVisible(true);
        ui->spinBox_directGD_inputY_minutes->setVisible(true);
        ui->spinBox_directGD_inputY_seconds->setVisible(true);
        ui->label_directGD_y_degree->setVisible(true);
        ui->label_directGD_y_minutes->setVisible(true);
        ui->label_directGD_y_seconds->setVisible(true);

        ui->lineEdit_resDirectGD_Bx->setVisible(false);
        ui->spinBox_resDirectGD_BX_degrees->setVisible(true);
        ui->label_resDirectGD_x_degree->setVisible(true);
        ui->spinBox_resDirectGD_BX_minutes->setVisible(true);
        ui->label_resDirectGD_x_minute->setVisible(true);
        ui->spinBox_resDirectGD_BX_seconds->setVisible(true);
        ui->label_resDirectGD_x_second->setVisible(true);

        ui->lineEdit_resDirectGD_By->setVisible(false);
        ui->spinBox_resDirectGD_BY_degrees->setVisible(true);
        ui->label_resDirectGD_y_degree->setVisible(true);
        ui->spinBox_resDirectGD_BY_minutes->setVisible(true);
        ui->label_resDirectGD_y_minute->setVisible(true);
        ui->spinBox_resDirectGD_BY_seconds->setVisible(true);
        ui->label_resDirectGD_y_second->setVisible(true);

        ui->lineEdit_directGD_inputAh->setVisible(false);
        ui->label_directGD_inputH->setVisible(false);
        ui->label_directGD_m_h->setVisible(false);
        ui->label_directGD_AngleH->setVisible(false);
        ui->label_directGD_degree_H->setVisible(false);
        ui->label_directGD_minutes_H->setVisible(false);
        ui->label_directGD_seconds_H->setVisible(false);
        ui->label_directGD_inputAngle_H->setVisible(false);
        ui->spinBox_directGD_inputAngle_degreesH->setVisible(false);
        ui->spinBox_directGD_inputAngle_minutesH->setVisible(false);
        ui->spinBox_directGD_inputAngle_secondsH->setVisible(false);
        ui->label_resDirectGD_BH->setVisible(false);
        ui->label_resDirectGD_coordB_H->setVisible(false);
        ui->label_resDirectGD_m_H->setVisible(false);
        ui->lineEdit_resDirectGD_BH->setVisible(false);
    }
    else if (index == 3)
    {//choose WGS-84
        ui->label_directGD_m_in_L->setText("км");
        ui->label_directGD_m_scale->setText("м     при масштабе       1:");
        ui->lineEdit_directGD_inputScale->setText(QString::number(1000));

        ui->lineEdit_directGD_inputAx->setVisible(false);
        ui->spinBox_directGD_inputX_degrees->setVisible(true);
        ui->spinBox_directGD_inputX_minutes->setVisible(true);
        ui->spinBox_directGD_inputX_seconds->setVisible(true);
        ui->label_directGD_x_degree->setVisible(true);
        ui->label_directGD_x_minutes->setVisible(true);
        ui->label_directGD_x_seconds->setVisible(true);

        ui->lineEdit_directGD_inputAy->setVisible(false);
        ui->spinBox_directGD_inputY_degrees->setVisible(true);
        ui->spinBox_directGD_inputY_minutes->setVisible(true);
        ui->spinBox_directGD_inputY_seconds->setVisible(true);
        ui->label_directGD_y_degree->setVisible(true);
        ui->label_directGD_y_minutes->setVisible(true);
        ui->label_directGD_y_seconds->setVisible(true);

        ui->lineEdit_resDirectGD_Bx->setVisible(false);
        ui->spinBox_resDirectGD_BX_degrees->setVisible(true);
        ui->label_resDirectGD_x_degree->setVisible(true);
        ui->spinBox_resDirectGD_BX_minutes->setVisible(true);
        ui->label_resDirectGD_x_minute->setVisible(true);
        ui->spinBox_resDirectGD_BX_seconds->setVisible(true);
        ui->label_resDirectGD_x_second->setVisible(true);

        ui->lineEdit_resDirectGD_By->setVisible(false);
        ui->spinBox_resDirectGD_BY_degrees->setVisible(true);
        ui->label_resDirectGD_y_degree->setVisible(true);
        ui->spinBox_resDirectGD_BY_minutes->setVisible(true);
        ui->label_resDirectGD_y_minute->setVisible(true);
        ui->spinBox_resDirectGD_BY_seconds->setVisible(true);
        ui->label_resDirectGD_y_second->setVisible(true);

        ui->lineEdit_directGD_inputAh->setVisible(false);
        ui->label_directGD_inputH->setVisible(false);
        ui->label_directGD_m_h->setVisible(false);
        ui->label_directGD_AngleH->setVisible(false);
        ui->label_directGD_degree_H->setVisible(false);
        ui->label_directGD_minutes_H->setVisible(false);
        ui->label_directGD_seconds_H->setVisible(false);
        ui->label_directGD_inputAngle_H->setVisible(false);
        ui->spinBox_directGD_inputAngle_degreesH->setVisible(false);
        ui->spinBox_directGD_inputAngle_minutesH->setVisible(false);
        ui->spinBox_directGD_inputAngle_secondsH->setVisible(false);
        ui->label_resDirectGD_BH->setVisible(false);
        ui->label_resDirectGD_coordB_H->setVisible(false);
        ui->label_resDirectGD_m_H->setVisible(false);
        ui->lineEdit_resDirectGD_BH->setVisible(false);
    }
}

//on invers page

void MainWindow::on_comboBox_inverseGD_currentIndexChanged(int index)
{
    if (index == 0)
    {//choose "Плоскость"
        ui->lineEdit_inverseGD_inputAx->setVisible(true);
        ui->spinBox_invresGD_inputAX_degrees->setVisible(false);
        ui->label_inversGD_Ax_degree->setVisible(false);
        ui->spinBox_invresGD_inputAX_minutes->setVisible(false);
        ui->label_inversGD_Ax_minute->setVisible(false);
        ui->spinBox_invresGD_inputAX_seconds->setVisible(false);
        ui->label_inversGD_Ax_second->setVisible(false);

        ui->lineEdit_inverseGD_inputAy->setVisible(true);
        ui->spinBox_invresGD_inputAY_degrees->setVisible(false);
        ui->label_inversGD_Ay_degree->setVisible(false);
        ui->spinBox_invresGD_inputAY_minutes->setVisible(false);
        ui->label_inversGD_Ay_minute->setVisible(false);
        ui->spinBox_invresGD_inputAY_seconds->setVisible(false);
        ui->label_inversGD_Ay_second->setVisible(false);

        ui->lineEdit_inverseGD_inputBx->setVisible(true);
        ui->spinBox_invresGD_inputBX_degrees->setVisible(false);
        ui->label_inversGD_Bx_degree->setVisible(false);
        ui->spinBox_invresGD_inputBX_minutes->setVisible(false);
        ui->label_inversGD_Bx_minute->setVisible(false);
        ui->spinBox_invresGD_inputBX_seconds->setVisible(false);
        ui->label_inversGD_Bx_second->setVisible(false);

        ui->lineEdit_inverseGD_inputBy->setVisible(true);
        ui->spinBox_invresGD_inputBY_degrees->setVisible(false);
        ui->label_inversGD_By_degree->setVisible(false);
        ui->spinBox_invresGD_inputBY_minutes->setVisible(false);
        ui->label_inversGD_By_minute->setVisible(false);
        ui->spinBox_invresGD_inputBY_seconds->setVisible(false);
        ui->label_inversGD_By_second->setVisible(false);

        ui->label_inverseGD_m_h->setVisible(false);
        ui->label_inverseGD_m_h_2->setVisible(false);
        ui->label_inverseGD_inputAH->setVisible(false);
        ui->label_inverseGD_inputBH->setVisible(false);
        ui->lineEdit_inverseGD_inputAH->setVisible(false);
        ui->lineEdit_inverseGD_inputBH->setVisible(false);
        ui->label_resInverseGD_AngleH->setVisible(false);
        ui->label_resInverse_degreeH->setVisible(false);
        ui->label_resInverseGD_angleH->setVisible(false);
        ui->label_resInverseGD_minutesH->setVisible(false);
        ui->label_resInverseGD_secondsH->setVisible(false);
        ui->spinBox_resInverseGD_Angle_degreesH->setVisible(false);
        ui->spinBox_resInverseGD_Angle_minutesH->setVisible(false);
        ui->spinBox_resInverseGD_Angle_secondsH->setVisible(false);

        ui->label_resInverseGD_m->setText("м");
    }
    else if (index == 1)
    {//choose "Плоскость с высотой"
        ui->lineEdit_inverseGD_inputAx->setVisible(true);
        ui->spinBox_invresGD_inputAX_degrees->setVisible(false);
        ui->label_inversGD_Ax_degree->setVisible(false);
        ui->spinBox_invresGD_inputAX_minutes->setVisible(false);
        ui->label_inversGD_Ax_minute->setVisible(false);
        ui->spinBox_invresGD_inputAX_seconds->setVisible(false);
        ui->label_inversGD_Ax_second->setVisible(false);

        ui->lineEdit_inverseGD_inputAy->setVisible(true);
        ui->spinBox_invresGD_inputAY_degrees->setVisible(false);
        ui->label_inversGD_Ay_degree->setVisible(false);
        ui->spinBox_invresGD_inputAY_minutes->setVisible(false);
        ui->label_inversGD_Ay_minute->setVisible(false);
        ui->spinBox_invresGD_inputAY_seconds->setVisible(false);
        ui->label_inversGD_Ay_second->setVisible(false);

        ui->lineEdit_inverseGD_inputBx->setVisible(true);
        ui->spinBox_invresGD_inputBX_degrees->setVisible(false);
        ui->label_inversGD_Bx_degree->setVisible(false);
        ui->spinBox_invresGD_inputBX_minutes->setVisible(false);
        ui->label_inversGD_Bx_minute->setVisible(false);
        ui->spinBox_invresGD_inputBX_seconds->setVisible(false);
        ui->label_inversGD_Bx_second->setVisible(false);

        ui->lineEdit_inverseGD_inputBy->setVisible(true);
        ui->spinBox_invresGD_inputBY_degrees->setVisible(false);
        ui->label_inversGD_By_degree->setVisible(false);
        ui->spinBox_invresGD_inputBY_minutes->setVisible(false);
        ui->label_inversGD_By_minute->setVisible(false);
        ui->spinBox_invresGD_inputBY_seconds->setVisible(false);
        ui->label_inversGD_By_second->setVisible(false);

        ui->label_inverseGD_m_h->setVisible(true);
        ui->label_inverseGD_m_h_2->setVisible(true);
        ui->label_inverseGD_inputAH->setVisible(true);
        ui->label_inverseGD_inputBH->setVisible(true);
        ui->lineEdit_inverseGD_inputAH->setVisible(true);
        ui->lineEdit_inverseGD_inputBH->setVisible(true);
        ui->label_resInverseGD_AngleH->setVisible(true);
        ui->label_resInverse_degreeH->setVisible(true);
        ui->label_resInverseGD_angleH->setVisible(true);
        ui->label_resInverseGD_minutesH->setVisible(true);
        ui->label_resInverseGD_secondsH->setVisible(true);
        ui->spinBox_resInverseGD_Angle_degreesH->setVisible(true);
        ui->spinBox_resInverseGD_Angle_minutesH->setVisible(true);
        ui->spinBox_resInverseGD_Angle_secondsH->setVisible(true);

        ui->label_resInverseGD_m->setText("м");
    }
    else if (index == 2)
    {//choose "Сфера"
        ui->lineEdit_inverseGD_inputAx->setVisible(false);
        ui->spinBox_invresGD_inputAX_degrees->setVisible(true);
        ui->label_inversGD_Ax_degree->setVisible(true);
        ui->spinBox_invresGD_inputAX_minutes->setVisible(true);
        ui->label_inversGD_Ax_minute->setVisible(true);
        ui->spinBox_invresGD_inputAX_seconds->setVisible(true);
        ui->label_inversGD_Ax_second->setVisible(true);

        ui->lineEdit_inverseGD_inputAy->setVisible(false);
        ui->spinBox_invresGD_inputAY_degrees->setVisible(true);
        ui->label_inversGD_Ay_degree->setVisible(true);
        ui->spinBox_invresGD_inputAY_minutes->setVisible(true);
        ui->label_inversGD_Ay_minute->setVisible(true);
        ui->spinBox_invresGD_inputAY_seconds->setVisible(true);
        ui->label_inversGD_Ay_second->setVisible(true);

        ui->lineEdit_inverseGD_inputBx->setVisible(false);
        ui->spinBox_invresGD_inputBX_degrees->setVisible(true);
        ui->label_inversGD_Bx_degree->setVisible(true);
        ui->spinBox_invresGD_inputBX_minutes->setVisible(true);
        ui->label_inversGD_Bx_minute->setVisible(true);
        ui->spinBox_invresGD_inputBX_seconds->setVisible(true);
        ui->label_inversGD_Bx_second->setVisible(true);

        ui->lineEdit_inverseGD_inputBy->setVisible(false);
        ui->spinBox_invresGD_inputBY_degrees->setVisible(true);
        ui->label_inversGD_By_degree->setVisible(true);
        ui->spinBox_invresGD_inputBY_minutes->setVisible(true);
        ui->label_inversGD_By_minute->setVisible(true);
        ui->spinBox_invresGD_inputBY_seconds->setVisible(true);
        ui->label_inversGD_By_second->setVisible(true);

        ui->label_inverseGD_m_h->setVisible(false);
        ui->label_inverseGD_m_h_2->setVisible(false);
        ui->label_inverseGD_inputAH->setVisible(false);
        ui->label_inverseGD_inputBH->setVisible(false);
        ui->lineEdit_inverseGD_inputAH->setVisible(false);
        ui->lineEdit_inverseGD_inputBH->setVisible(false);
        ui->label_resInverseGD_AngleH->setVisible(false);
        ui->label_resInverse_degreeH->setVisible(false);
        ui->label_resInverseGD_angleH->setVisible(false);
        ui->label_resInverseGD_minutesH->setVisible(false);
        ui->label_resInverseGD_secondsH->setVisible(false);
        ui->spinBox_resInverseGD_Angle_degreesH->setVisible(false);
        ui->spinBox_resInverseGD_Angle_minutesH->setVisible(false);
        ui->spinBox_resInverseGD_Angle_secondsH->setVisible(false);

        ui->label_resInverseGD_m->setText("км");
    }
}
