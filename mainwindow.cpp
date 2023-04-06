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

    //check radioButton
    int scale;
    if (!ui->radioButton_directGD_inputHorizSpacing->isChecked())
    {
        scale = ui->lineEdit_directGD_inputScale->text().toInt(); //scale
        distance = distance * scale / 100;
    }
    else
        scale = 100;


    switch (ui->comboBox_directGD->currentIndex())
    {//selected area
    case 0:
    {//plane
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
    case 1:
    {//plane with height

        double height = ui->lineEdit_directGD_inputAh->text().toDouble();

        double angleH, heightB;
        double degreesH = ui->spinBox_directGD_inputAngle_degreesH->text().toDouble(); //angle degrees
        double minutesH = ui->spinBox_directGD_inputAngle_minutesH->text().toDouble(); //angle minutes
        double secondsH = ui->spinBox_directGD_inputAngle_secondsH->text().toDouble(); //angle seconds

        //angle
        angleOfDMS(degrees, minutes, seconds, angle);
        //angleH
        angleOfDMS(degreesH, minutesH, secondsH, angleH);

        //solve directGDwithH
        directGDPlaneH(latA, lonA, height, angle, angleH, distance, latB, lonB, heightB);

        //rounding to 2 digits after
        ui->lineEdit_resDirectGD_Bx->setText(QString::number(latB, 'f', 2));
        ui->lineEdit_resDirectGD_By->setText(QString::number(lonB, 'f', 2));
        ui->lineEdit_resDirectGD_BH->setText(QString::number(heightB, 'f', 2));

        go_next();

        break;
    }
    case 2:
    {//sphere
        angleOfDMS(degrees, minutes, seconds, angle);

        directGDSphera(latA, lonA, angle, distance, latB, lonB);

        ui->lineEdit_resDirectGD_Bx->setText(QString::number(latB));
        ui->lineEdit_resDirectGD_By->setText(QString::number(lonB));
        //        double reverseAngle;
        //        double angle;
        //        double pt1[2], pt2[2];

        //        angleImDMS(degrees, minutes, seconds, angle);

        //        pt1[0] = Radians(latA);
        //        pt1[1] = Radians(lonA);
        //        sphereDirect(pt1, Radians(angle), distance / A_E, pt2);

        //        //rounding to 2 digits after
        //        ui->lineEdit_resDirectGD_Bx->setText(QString::number(Degrees(pt2[0])));
        //        ui->lineEdit_resDirectGD_By->setText(QString::number(Degrees(pt2[1])));
        ////        ui->lineEdit_resDirectGD_reverseAngle->setText(QString::number(reverseAngle));

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
    double distance, degrees, minutes, seconds, angle;
    double lat[2], lon[2];
    lat[0] = ui->lineEdit_inverseGD_inputAx->text().toDouble(); //coordinates of point A (x)
    lon[0] = ui->lineEdit_inverseGD_inputAy->text().toDouble(); //coordinates of point A (y)
    lat[1] = ui->lineEdit_inverseGD_inputBx->text().toDouble(); //coordinates of point A (x)
    lon[1] = ui->lineEdit_inverseGD_inputBy->text().toDouble(); //coordinates of point A (y)

    switch (ui->comboBox_inverseGD->currentIndex())
    {//selected area
    case 0:
    {//plane
        //solve invresGD
        inversGDPlane(lat, lon, angle, distance);

        angleFromDecimalAngle(angle, degrees, minutes, seconds);
        if (degrees < 0) degrees += 360;

        ui->lineEdit_resInverseGD_outputL->setText(QString::number(distance, 'f', 2)); //distance output rounded to 2 decimal places
        ui->spinBox_resInverseGD_Angle_degrees->setValue(int(degrees));
        ui->spinBox_resInverseGD_Angle_minutes->setValue(int(minutes));
        ui->spinBox_resInverseGD_Angle_seconds->setValue(int(seconds));

        go_next();
        break;
    }
    case 1:
    {//plane with height

        double height[2], angleOrent;
        height[0] = ui->lineEdit_inverseGD_inputAH->text().toDouble();
        height[1] = ui->lineEdit_inverseGD_inputBH->text().toDouble();

        //solve inversGD
        inversGDPlaneH(lat, lon, height, angle, distance, angleOrent);

        angleFromDecimalAngle(angle, degrees, minutes, seconds);
        if (degrees < 0) degrees += 360;

        ui->lineEdit_resInverseGD_outputL->setText(QString::number(distance, 'f', 2)); //distance output rounded to 2 decimal places
        ui->spinBox_resInverseGD_Angle_degrees->setValue(int(degrees));
        ui->spinBox_resInverseGD_Angle_minutes->setValue(int(minutes));
        ui->spinBox_resInverseGD_Angle_seconds->setValue(int(seconds));

        angleFromDecimalAngle(angleOrent, degrees, minutes, seconds);
        ui->spinBox_resInverseGD_Angle_degreesH->setValue(int(degrees));
        ui->spinBox_resInverseGD_Angle_minutesH->setValue(int(minutes));
        ui->spinBox_resInverseGD_Angle_secondsH->setValue(int(seconds));

        go_next();
        break;
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

    //resDirectGD

    ui->label_resDirectGD_BH->setVisible(false);
    ui->label_resDirectGD_coordB_H->setVisible(false);
    ui->label_resDirectGD_m_H->setVisible(false);
    ui->lineEdit_resDirectGD_BH->setVisible(false);

    //inverseGD

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

void MainWindow::on_comboBox_directGD_currentIndexChanged(int index)
{
    if (index == 1)
    {//choose "Плоскость с высотой"
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
    else
    {
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

void MainWindow::on_comboBox_inverseGD_currentIndexChanged(int index)
{
    if (index == 1)
    {//choose "Плоскость с высотой"
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
    }
    else
    {
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
    }
}
