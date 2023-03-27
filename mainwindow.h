#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <iostream>
#include <QtWidgets>
#include <cmath>
#include <qmath.h>
#include <QValidator>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void on_pushButton_directGD_main_clicked(); //button "Прямая геодезическая задача" in main

    void on_pushButton_directGD_back_clicked(); //button "Назад" in directGD

    void on_pushButton_directGD_solve_clicked(); //button "Решить" in directGD

    void on_pushButton_resDirectGD_back_clicked(); //button "Назад" in resDirectGD

    void on_pushButton_resDirectGD_goToMain_clicked(); //button "На главную страницу" in resDirectGD

    void on_lineEdit_directGD_inputScale_textChanged(const QString &arg1); //change lineEdit scale in directGD

    void go_main(); //func to go to main

    void go_back(); //func to go back

    void go_next(); //func to go next

    void setValidator(); //func for set Validator in lineEdit and spinBox

    void on_pushButton_reverseGD_back_clicked();

    void on_pushButton_inverseGD_main_clicked();

    void on_pushButton_reverseGD_solve_clicked();

    void on_pushButton_resReverseGD_back_clicked();

    void on_pushButton_resReverseGD_goToMain_clicked();

    void on_lineEdit_reversGD_inputAx_textChanged(const QString &arg1);

    void on_lineEdit_reversGD_inputBx_textChanged(const QString &arg1);

    void on_lineEdit_reversGD_inputAy_textChanged(const QString &arg1);

    void on_lineEdit_reversGD_inputBy_textChanged(const QString &arg1);

    void on_lineEdit_directGD_inputAx_textChanged(const QString &arg1);

    void on_lineEdit_directGD_inputAy_textChanged(const QString &arg1);

    void on_lineEdit_directGD_inputL_textChanged(const QString &arg1);

    void on_spinBox_directGD_inputAngle_degrees_valueChanged(int arg1);

    void on_spinBox_directGD_inputAngle_minutes_valueChanged(int arg1);

    void on_spinBox_directGD_inputAngle_seconds_valueChanged(int arg1);


private:
    Ui::MainWindow *ui;

    void directGD(double latA, double lonA, double angle, double distance, double& latB, double& lonB)
    {
        double radians = angle * M_PI / 180;

        double deltaX = distance * cos(radians);
        double deltaY = distance * sin(radians);

        latB = latA + deltaX;
        lonB = lonA + deltaY;
    }

    void angleImDMS(double degrees, double minutes, double seconds, double& angle)
    {
        angle = degrees + (minutes / 60.0) + (seconds / 3600.0);
    }

    void realSize(double distance, int scale, double& scaleDistance)
    {
        scaleDistance = (distance * scale) / 100;
    }

    void directGD_solve(double latA, double lonA, double degrees, double minutes, double seconds, double distance, int scale, double& latB, double& lonB)
    {
        double scaleDistance;
        double angle;

        //solve scale with distance

        realSize(distance, scale, scaleDistance);

        //solve angle

        angleImDMS(degrees, minutes, seconds, angle); //angle of degrees, minutes, seconds

        //solve directGD

        directGD(latA, lonA, angle, scaleDistance, latB, lonB);
    }

    void reverseGD_solve(double latA, double lonA, double latB, double lonB, double& distance, double& degrees, double& seconds, double& minutes)
    {
        double angle;

        reversGD(latA, lonA, latB, lonB, distance, angle);

        angleFromDecimalAngle(angle, degrees, seconds, minutes);

        if (degrees < 0) degrees += 360;

    }

    void reversGD(double latA, double lonA, double latB, double lonB, double& distance, double& angle)
    {
        double deltaX = latB - latA;
        double deltaY = lonB - lonA;

        angle = (atan(deltaY / deltaX) * 180 / M_PI);

        distance = sqrt(deltaX*deltaX + deltaY*deltaY);

        if (deltaX < 0 and deltaY > 0) angle -= 180;
        else if (deltaX < 0 and deltaY < 0) angle += 180;
        else if (deltaX > 0 and deltaY < 0) return;
    }

    void angleFromDecimalAngle(double angle, double& degrees, double& minutes, double& seconds)
    {
        degrees = floor(angle);
        double tmpMinutes = (angle - degrees) * 60;

        minutes = floor(tmpMinutes);
        double tmpSeconds = (tmpMinutes - minutes) * 60;

        seconds = round(tmpSeconds);
    }
};

#endif // MAINWINDOW_H

