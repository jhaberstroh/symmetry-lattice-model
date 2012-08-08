#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <iostream>
#include <QMainWindow>
#include <QSlider>
#include <QPushButton>
#include <QString>
#include <QTextStream>
#include <QThread>
#include <QDebug>
#include <QLabel>
#include "functionthread.h"
//#include "../Metropolis_Growth/montecarlo.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT


public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_go_toggled(bool checked);
    void on_parameter_value_changed(int value);
    void on_reset_clicked(bool checked);
    void on_thread_returned_value(QString image_location);

signals:
    void parameter_changed(double parameter_value, FunctionThread::Parameter p);

private:
    double SliderToDouble(int);

    double get_j();
    double get_qn1();
    double get_qn2();
    double get_pdel();
    double get_T();
    void UpdateMCValuesRuntime();
    void UpdateMCValuesReset();

    Ui::MainWindow *ui;
    FunctionThread* ft;
    bool open_thread;
};

#endif // MAINWINDOW_H
