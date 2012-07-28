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

    void on_thread_returned_value(double vals);

signals:
    void parameter_changed(double parameter_value, FunctionThread::Parameter p);

private:
    double SliderToDouble(int);

    //    MonteCarlo* m;
    Ui::MainWindow *ui;
    FunctionThread* ft;
    bool open_thread;
};

#endif // MAINWINDOW_H
