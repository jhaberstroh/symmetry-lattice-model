#ifndef FUNCTIONTHREAD_H
#define FUNCTIONTHREAD_H

#include <QPushButton>
#include <QThread>
#include <QString>
#include <QDebug>
#include "simpleclass.h"
#include "montecarlo.h"


class FunctionThread : public QThread
{
    Q_OBJECT
public:
    enum Parameter{kParamJ, kParamQN1, kParamQN2, kNone};
    explicit FunctionThread(QObject *parent = 0, QPushButton* go = 0, double J=0, double QN1=0, double QN2=0, int sweeps=10, double delay_seconds =1,
                            int size_x = 25, int size_y = 25, int R = 8, int N1 = 2, int N2 = 4);
    void beginMCRunning();

signals:
    void sendOutput(QString image_location);

public slots:
    void on_go_toggled(bool checked);
//    void on_parameter_changed(double new_parameter_value, Parameter p);


protected:
    friend class MainWindow;
    void OutputLatticeImage();
    void run();

    QPushButton* m_go;
    MonteCarlo m_montecarlo;
    int m_count;
    bool m_open_thread;
    int m_sweeps_between_image;
    double m_delay_seconds;

public:
    double J(){return m_montecarlo.J();}
    double QN1(){return m_montecarlo.QN1();}
    double QN2(){return m_montecarlo.QN2();}
    bool has_open_thread(){return m_open_thread;}
};

#endif // FUNCTIONTHREAD_H
