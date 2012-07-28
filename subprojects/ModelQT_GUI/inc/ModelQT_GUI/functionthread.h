#ifndef FUNCTIONTHREAD_H
#define FUNCTIONTHREAD_H

#include <QPushButton>
#include <QThread>
#include <QDebug>
#include "simpleclass.h"

class FunctionThread : public QThread
{
    Q_OBJECT
public:
    enum Parameter{kParamJ, kParamQN1, kParamQN2, kNone};
    explicit FunctionThread(QObject *parent = 0, QPushButton* go = 0);
    void beginMCRunning();
    
signals:
    void sendOutput(double output);
    
public slots:
    void on_go_toggled(bool checked);
    void on_parameter_changed(double new_parameter_value, Parameter p);

protected:
    void run();

    QPushButton* m_go;
    SimpleClass m_s;
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
