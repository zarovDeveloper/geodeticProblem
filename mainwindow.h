#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <iostream>
#include <cmath>
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

    void go_main();

    void go_back();

    void directGD_solve();

    void on_pushButton_directGDres_back_clicked();

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

};

#endif // MAINWINDOW_H
