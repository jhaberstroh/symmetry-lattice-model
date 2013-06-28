#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <iostream>
#include <QMainWindow>
#include <QSlider>
#include <QPushButton>
#include <QRadioButton>
#include <QComboBox>
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
    void on_J_value_changed(int value);
    void on_QN1_value_changed(int value);
    void on_QN2_value_changed(int value);
    void on_reset_clicked(bool checked);
    void on_thread_returned_value(QString image_location);
    void redraw();

signals:
    void valueChanged(double parameter_value, FunctionThread::Parameter p);

private:
    double SliderToDouble(int);

    double get_j();
    double get_qn1();
    double get_qn2();
    double get_pdel();
    double get_T();
    int get_R();
    int get_N1();
    int get_N2();
    void UpdateMCValuesRuntime();
    void UpdateMCValuesReset();

    Ui::MainWindow *ui;
    FunctionThread* ft;
    bool reset_queued;
};

#endif // MAINWINDOW_H
