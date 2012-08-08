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
    explicit FunctionThread(QObject *parent = 0, QPushButton* go = 0);
    void beginMCRunning();

signals:
    void sendOutput(QString image_location);

public slots:
    void on_go_toggled(bool checked);
    void on_parameter_changed(double new_parameter_value, Parameter p);


protected:
    friend class MainWindow;
    void run();

    QPushButton* m_go;
    MonteCarlo m_montecarlo;
    int m_count;
    double m_J;
    double m_QN1;
    double m_QN2;

public:
    double J(){return m_J;}
    double QN1(){return m_QN1;}
    double QN2(){return m_QN2;}
};

#endif // FUNCTIONTHREAD_H
