#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->go->setCheckable(true);
    ui->stop->setCheckable(true);
    ui->MainDisplay->setBackgroundRole(QPalette::Base);
    ui->MainDisplay->setScaledContents(true);
    ui->MainDisplay->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);

    ft = new FunctionThread(0, ui->go);
    open_thread = 0;

    connect(ui->go, SIGNAL(toggled(bool)), ft, SLOT(on_go_toggled(bool)));
    connect(ft, SIGNAL(sendOutput(QString)), this, SLOT(on_thread_returned_value(QString)));
    connect(ui->J, SIGNAL(valueChanged(int)), this, SLOT(on_parameter_value_changed(int)));
    connect(ui->QN1, SIGNAL(valueChanged(int)), this,  SLOT(on_parameter_value_changed(int)));
    connect(ui->QN2, SIGNAL(valueChanged(int)), this,  SLOT(on_parameter_value_changed(int)));
    connect(ui->reset,SIGNAL(clicked(bool)), this, SLOT(on_reset_clicked(bool)));
    //connect(this, SIGNAL(parameter_changed(double,FunctionThread::Parameter)),
    //        ft, SLOT(on_parameter_changed(double, FunctionThread::Parameter)));

    for (int i = 0 ; i < Lattice::N_PHASES ; i++){
        ui->default_phase->addItem(ft->m_montecarlo.PhaseStringLookup(i),i);
    }
}

MainWindow::~MainWindow()
{
    delete ft;
    delete ui;
}


double MainWindow::SliderToDouble(int slider_value){
    return double(slider_value) / 1000;
}


double MainWindow::get_j(){ return SliderToDouble(ui->J->value());}
double MainWindow::get_qn1(){return SliderToDouble(ui->QN1->value());}
double MainWindow::get_qn2(){return SliderToDouble(ui->QN2->value());}
double MainWindow::get_pdel(){return ui->pdel_spin_box->value();}
double MainWindow::get_T(){return ui->T_spin_box->value();};

void MainWindow::UpdateMCValuesRuntime(){
    ft->m_montecarlo.set_j(get_j());
    ft->m_montecarlo.set_qN1(get_qn1());
    ft->m_montecarlo.set_qN2(get_qn2());
    ft->m_montecarlo.set_pdel(get_pdel());
    ft->m_montecarlo.set_T(get_T());
}

void MainWindow::UpdateMCValuesReset(){
    UpdateMCValuesRuntime();
}




/*-------------------------------------------
    SLOTS
    -----------------------------------------*/

void MainWindow::on_go_toggled(bool checked)
{
    QString txt;
    QTextStream txt_buf(&txt);
    txt_buf << "Paused: " << ui->go->isChecked();
    qDebug() << *txt_buf.string();
    ui->dialogueText->setText(*txt_buf.string());
}

void MainWindow::on_reset_clicked(bool checked)
{


}


void MainWindow::on_parameter_value_changed(int value)
{
 // When sliders change, disable the running.
    //ui->go->setChecked(false);
    //QString txt;
    //QTextStream txt_buf(&txt);
    //txt_buf << "Paused: " << !ui->go->isChecked();
    //ui->dialogueText->setText(*txt_buf.string());

    if (SliderToDouble(ui->J->value()) != ft->J()){
        qDebug() << "J_value_changed";
        ui->labelJ->setNum(get_j());
        ft->on_parameter_changed(get_j(), FunctionThread::kParamJ);
    }
    if (SliderToDouble(ui->QN1->value()) != ft->QN1()){
        qDebug() << "QN1_value_changed";
        ui->labelQN1->setNum(get_qn1());
        ft->on_parameter_changed(get_qn1(),FunctionThread::kParamQN1);
    }
    if (SliderToDouble(ui->QN2->value()) != ft->QN2()){
        qDebug() << "QN2_value_changed";
        ui->labelQN2->setNum(get_qn2());
        ft->on_parameter_changed(get_qn2(),FunctionThread::kParamQN2);
    }
}



void MainWindow::on_thread_returned_value(QString image_name)
{
    qDebug() << "Thread returned "<< image_name;
    QString txt;
    QTextStream txt_buf(&txt);
    txt_buf << "Return Value: " << image_name;
    //TODO: make MainDisplay show the last created image file
    image_name += ".png";
    QImage image(image_name);
    if (!image.isNull()){
        ui->MainDisplay->setPixmap(QPixmap::fromImage(image));
        Q_ASSERT(ui->MainDisplay->pixmap());
    }
    UpdateMCValuesRuntime();
}

