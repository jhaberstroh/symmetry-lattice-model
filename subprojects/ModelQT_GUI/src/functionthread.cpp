#include "functionthread.h"

FunctionThread::FunctionThread(QObject *parent, QPushButton* go) :
    QThread(parent), m_go(go), m_count(0),
    m_J(0), m_QN1(0), m_QN2(0)
{
    m_montecarlo.set_j(m_J);
    m_montecarlo.set_qN1(m_QN1);
    m_montecarlo.set_qN2(m_QN2);
    m_montecarlo.reset_default_phase(Lattice::LIQUID);
    m_montecarlo.order_parameter_handler().Track();
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
    default:
        qDebug() << "Recieved an erroneous  or not-yet-handled parameter change.";
    }
}


void FunctionThread::run(){
    while (m_go->isChecked()){
        sleep(1);
        m_montecarlo.set_j(m_J);
        m_montecarlo.set_qN1(m_QN1);
        m_montecarlo.set_qN2(m_QN2);

        qDebug() << "count = " << m_count++;
        for (int i = 0 ; i < 10 ; i++){
            m_montecarlo.DoMetropolisSweep();
            m_montecarlo.PrintTextLattice();
        }
        m_montecarlo.order_parameter_handler().Track();
        m_montecarlo.order_parameter_handler().MakeOPImage();
        emit sendOutput(rand());
    }
}



