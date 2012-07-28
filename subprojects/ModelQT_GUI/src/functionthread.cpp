#include "functionthread.h"

FunctionThread::FunctionThread(QObject *parent, QPushButton* go) :
    QThread(parent), m_s(), m_go(go), m_count(0),
    m_J(0), m_QN1(0), m_QN2(0)
{
}


void FunctionThread::beginMCRunning(){
    start();
}

void FunctionThread::on_go_toggled(bool checked){
    if (checked)
        beginMCRunning();
}

void FunctionThread::on_parameter_changed(double new_parameter_value, Parameter p){
    qDebug()<<"Recieved on_parameter_changed signal in FunctionThread!";
    switch (p){
    case kParamJ:
        m_J = new_parameter_value;
        qDebug() << "new J value = "<<m_J;
        //m.UpdateJ(QN2);
        break;
    case kParamQN1:
        m_QN1 = new_parameter_value;
        qDebug() << "new QN1 value = "<<m_QN1;
        //m.UpdateQN1(QN2);
        break;
    case kParamQN2:
        m_QN2 = new_parameter_value;
        qDebug() << "new QN2 value = "<<m_QN2;
        //m.UpdateQN2(QN2);
        break;
    }
}


void FunctionThread::run(){
    while (m_go->isChecked()){
        sleep(1);
        qDebug() << "count = " << m_count++;
        emit sendOutput(rand());
    }
}



