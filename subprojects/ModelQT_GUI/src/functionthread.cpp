#include "functionthread.h"

FunctionThread::FunctionThread(QObject *parent, QPushButton* go, double J, double QN1, double QN2, int sweeps, double delay_seconds, int size_x, int size_y, int R, int N1, int N2) :
    QThread(parent), m_go(go), m_count(0), m_open_thread(false),
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


void FunctionThread::OutputLatticeImage(){
    m_montecarlo.MakeLatticePNG();
    //TODO: Insert "if radiobutton.Lattice" vs. "if radiobutton.OrderParameter"
    //If radiobutton.Lattice :
    QString emitter("./testing");
    //If radiobutton.OrderParameter :
    //QString emitter(m_montecarlo.op_image_location().c_str())
    qDebug() << emitter;
    qDebug() << "In FunctionThread::OutputLatticeImage(), m_go->isChecked() = " << m_go->isChecked();
    emit sendOutput(emitter);
}

void FunctionThread::run(){
    qDebug() << "In FunctionThread::run(), m_go->isChecked() = " << m_go->isChecked();
    while (m_go->isChecked()){
        sleep(m_delay_seconds);

        //qDebug() << "count = " << m_count++;
        for (int i = 0 ; i < m_sweeps_between_image && m_go->isChecked() ; i++){
            m_montecarlo.DoMetropolisSweep();
            //m_montecarlo.PrintTextLattice();
        }
        m_montecarlo.order_parameter_handler().Track();
        m_montecarlo.order_parameter_handler().MakeImage();
        OutputLatticeImage();
    }
    cout << "In FunctionThread::run(), m_open_thread set false" << endl;
    m_open_thread = false;
}



