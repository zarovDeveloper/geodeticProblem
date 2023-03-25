#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <iostream>
#include <QtWidgets>
#include <cmath>
#include <qmath.h>
#include <QValidator>

#define PI 3.14159265

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

    void on_pushButton_directGD_main_clicked();

    void on_pushButton_directGD_back_clicked();

    void on_pushButton_directGD_solve_clicked();

    void on_pushButton_directGDres_goToMain_clicked();

    void on_pushButton_directGDres_back_clicked();

    void go_main();

    void go_back();

    void setValidator();

    void on_lineEdit_directGD_inputScale_textChanged(const QString &arg1);

private:
    Ui::MainWindow *ui;

    void directGD(double latA, double lonA, double angle, double distance, double& latB, double& lonB)
    {
        double radians = angle * PI / 180;

        double deltaX = distance * cos(radians);
        double deltaY = distance * sin(radians);

        latB = latA + deltaX;
        lonB = lonA + deltaY;
    }

    void angleOfDMS(double degrees, double minutes, double seconds, double& angle)
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

        angleOfDMS(degrees, minutes, seconds, angle); //angle of degrees, minutes, seconds

        //solve directGD

        directGD(latA, lonA, angle, scaleDistance, latB, lonB);
    }

};

#endif // MAINWINDOW_H
