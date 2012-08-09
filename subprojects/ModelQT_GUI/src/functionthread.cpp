#include "functionthread.h"

FunctionThread::FunctionThread(QObject *parent, QPushButton* go, QRadioButton* lat_radio, double J, double QN1, double QN2, int sweeps, double delay_seconds, int size_x, int size_y, int R, int N1, int N2) :
    QThread(parent), m_go(go), m_lat_radio(lat_radio), m_count(0), m_open_thread(false),
    m_sweeps_between_image(sweeps), m_delay_seconds(delay_seconds)
{
    m_montecarlo.set_j(J);
    m_montecarlo.set_qN1(QN1);
    m_montecarlo.set_qN2(QN2);
    m_montecarlo.reset_full(Lattice::LIQUID, R, N1, N2, size_x, size_y);
    m_montecarlo.order_parameter_handler().Track();
}


void FunctionThread::beginMCRunning(){
    qDebug()<< "TRYING TO START() THREAD";
    start();
}

void FunctionThread::on_go_toggled(bool checked){
    if (checked && !m_open_thread){
        beginMCRunning();
        m_open_thread = true;
    }
}


void FunctionThread::OutputDisplayImage(){

    if (m_lat_radio->isChecked()){
        QString name("./.tempdisplay.png");
        string std_name = name.toStdString();
        m_montecarlo.MakeLatticePNG(&std_name);
        emit sendOutput(name);
    }
    else{
        QString name("testing.png");
        string std_name = name.toStdString();
        m_montecarlo.order_parameter_handler().MakeImage(&std_name);
        emit sendOutput(name);
    }

}

void FunctionThread::run(){
    //qDebug() << "In FunctionThread::run(), m_go->isChecked() = " << m_go->isChecked();
    while (m_go->isChecked()){

        //qDebug() << "count = " << m_count++;
        for (int i = 0 ; i < m_sweeps_between_image && m_go->isChecked() ; i++){
            m_montecarlo.DoMetropolisSweep();
            //m_montecarlo.PrintTextLattice();
        }
        m_montecarlo.order_parameter_handler().Track();
        OutputDisplayImage();
        sleep(m_delay_seconds);
    }
    //qDebug() << "In FunctionThread::run(), m_open_thread set false";
    m_open_thread = false;
}



