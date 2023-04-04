#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <iostream>
#include <QtWidgets>
#include <cmath>
#include <qmath.h>
#include "geodeticproblem.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow, public geodeticProblem
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

    void on_pushButton_inverseGD_back_clicked();

    void on_pushButton_inverseGD_main_clicked();

    void on_pushButton_inverseGD_solve_clicked();

    void on_pushButton_resInverseGD_back_clicked();

    void on_pushButton_resInverseGD_goToMain_clicked();

    void on_lineEdit_inverseGD_inputAx_textChanged(const QString &arg1);

    void on_lineEdit_inverseGD_inputBx_textChanged(const QString &arg1);

    void on_lineEdit_inverseGD_inputAy_textChanged(const QString &arg1);

    void on_lineEdit_inverseGD_inputBy_textChanged(const QString &arg1);

    void on_lineEdit_directGD_inputAx_textChanged(const QString &arg1);

    void on_lineEdit_directGD_inputAy_textChanged(const QString &arg1);

    void on_lineEdit_directGD_inputL_textChanged(const QString &arg1);

    void on_spinBox_directGD_inputAngle_degrees_valueChanged(int arg1);

    void on_spinBox_directGD_inputAngle_minutes_valueChanged(int arg1);

    void on_spinBox_directGD_inputAngle_seconds_valueChanged(int arg1);

    void on_radioButton_directGD_inputL_clicked();

    void on_radioButton_directGD_inputHorizSpacing_clicked();

    void on_comboBox_directGD_currentIndexChanged(int index);

    void on_spinBox_directGD_inputAngle_degreesH_valueChanged(int arg1);

    void on_spinBox_directGD_inputAngle_minutesH_valueChanged(int arg1);

    void on_spinBox_directGD_inputAngle_secondsH_valueChanged(int arg1);

    void on_comboBox_inverseGD_currentIndexChanged(int index);

private:
    Ui::MainWindow *ui;

    void angleOfDMS(double degrees, double minutes, double seconds, double& angle)
    {
        angle = degrees + (minutes / 60.0) + (seconds / 3600.0);
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

