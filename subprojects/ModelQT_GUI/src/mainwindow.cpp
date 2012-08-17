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

    ft = new FunctionThread(0, ui->go, ui->lattice_radio_button, ui->OP_combo_box, get_j(), get_qn1(), get_qn2(),
                            ui->sweeps_spin_box->value(),ui->delay_spin_box->value(),
                            ui->x_spin_box->value(), ui->y_spin_box->value(), get_R(), get_N1(), get_N2());
    reset_queued = false;

    connect(ui->go, SIGNAL(toggled(bool)), ft, SLOT(on_go_toggled(bool)));
    connect(ft, SIGNAL(sendOutput(QString)), this, SLOT(on_thread_returned_value(QString)));
    connect(ui->J, SIGNAL(valueChanged(int)), this, SLOT(on_J_value_changed(int)));
    connect(ui->QN1, SIGNAL(valueChanged(int)), this,  SLOT(on_QN1_value_changed(int)));
    connect(ui->QN2, SIGNAL(valueChanged(int)), this,  SLOT(on_QN2_value_changed(int)));
    connect(ui->reset,SIGNAL(clicked(bool)), this, SLOT(on_reset_clicked(bool)));
    connect(ui->lattice_radio_button, SIGNAL(toggled(bool)), this, SLOT(redraw()));
    connect(ui->OP_combo_box, SIGNAL(currentIndexChanged(int)),this, SLOT(redraw()));
    //connect(this, SIGNAL(parameter_changed(double,FunctionThread::Parameter)),
    //        ft, SLOT(on_parameter_changed(double, FunctionThread::Parameter)));

    for (int i = 0 ; i < Lattice::N_PHASES ; i++){
        ui->default_phase->addItem(ft->m_montecarlo.PhaseStringLookup(i).c_str(),i);
    }
    for (int i = 0 ; i < OrderParamFile::kNColumns; i++){
        ui->OP_combo_box->addItem(ft->m_montecarlo.FColumnStringLookup(i).c_str(),i);
    }

    ft->OutputDisplayImage();
}

MainWindow::~MainWindow()
{
    ui->go->setChecked(false);
    while (ft->has_open_thread()){}
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
double MainWindow::get_T(){return ui->T_spin_box->value();}
int MainWindow::get_R(){return ui->R_spin_box->value();}
int MainWindow::get_N1(){return ui->N1_spin_box->value();}
int MainWindow::get_N2(){return ui->N2_spin_box->value();}

void MainWindow::UpdateMCValuesRuntime(){
    ft->m_montecarlo.set_j(get_j());
    ft->m_montecarlo.set_qN1(get_qn1());
    ft->m_montecarlo.set_qN2(get_qn2());
    ft->m_montecarlo.set_pdel(get_pdel());
    ft->m_montecarlo.set_T(get_T());
    ft->m_sweeps_between_image = ui->sweeps_spin_box->value();
    ft->m_delay_seconds = ui->delay_spin_box->value();
}

void MainWindow::UpdateMCValuesReset(){
    qDebug() << "Should be displaying image right about now";
    UpdateMCValuesRuntime();
    //Most round-about conversion ever...
    ft->m_montecarlo.reset_full(ft->m_montecarlo.IntToPhase(ui->default_phase->itemData(ui->default_phase->currentIndex()).toInt()),
                                get_R(), get_N1(), get_N2(), ui->x_spin_box->value(), ui->y_spin_box->value());
    redraw();
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
    if (ft->has_open_thread()){
        reset_queued = true;
        ui->go->setChecked(false);
    }
    else
        UpdateMCValuesReset();
}

void MainWindow::redraw(){
    ft->OutputDisplayImage();
}

void MainWindow::on_J_value_changed(int value){
    ui->labelJ->setNum(get_j());
}

void MainWindow::on_QN1_value_changed(int value){
    ui->labelQN1->setNum(get_qn1());
}

void MainWindow::on_QN2_value_changed(int value){
    ui->labelQN2->setNum(get_qn2());
}

void MainWindow::on_thread_returned_value(QString image_name)
{
    //TODO: could optimize with a "parameter-changed" variable
    if (reset_queued){
        while (ft->has_open_thread()){/*wait until the current thread completes*/}
        cout << "In MainWindow::on_thread_returned_value(), UpdateMCValuesReset() to be run" << endl;
        reset_queued = false; //Must be before UpdateMCValuesReset() in order to avoid recursion
        UpdateMCValuesReset();
        ui->go->setChecked(true);
    }
    else{
        UpdateMCValuesRuntime();
    }

    qDebug() << "Thread returned "<< image_name;
    QString txt;
    QTextStream txt_buf(&txt);
    txt_buf << "Return Value: " << image_name;
    //TODO: make MainDisplay show the last created image file
    QImage image(image_name);
    if (!image.isNull()){
        ui->MainDisplay->setPixmap(QPixmap::fromImage(image));
        Q_ASSERT(ui->MainDisplay->pixmap());
    }

    if (reset_queued){
        UpdateMCValuesReset();
        reset_queued = false;
    }
    else{
        UpdateMCValuesRuntime();
    }

}

