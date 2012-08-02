#include "orderparamfile.h"
#include "montecarlo.h"

using namespace std;
/*--------------------------------------------------
  OrderParamFile Members
  --------------------------------------------------*/

string OrderParamFile::MakeFileName(const vector<FNameOpt>& fname_include){
  ostringstream strbuf("");
  strbuf << "ordPar";
  for (unsigned int i = 0 ; i < fname_include.size() ; i++){
    switch (fname_include[i]){
    case kR:
      strbuf << "_R"<< m_montecarlo.R();
      break;
    case kJ:
      strbuf << "_J-"<<m_montecarlo.J();
      break;
    case kQN1:
      strbuf << "_Q"<<m_montecarlo.N1_symmetry_num()<<"-"<<m_montecarlo.QN1();
      break;
    case kQN2:
      strbuf << "_Q"<<m_montecarlo.N2_symmetry_num()<<"-"<<m_montecarlo.QN2();
      break;
    case kT:
      strbuf << "_T-"<<m_montecarlo.T();
      break;
    case kPDel:
      strbuf << "_PDEL-"<<m_montecarlo.pdel();
      break;
    default:
      strbuf << "_na_";
      break;
    }
  }
  string file_name = strbuf.str();
  FindIndexedName(&file_name, "csv");
  return file_name + ".csv";
}


void OrderParamFile::InsertHeader(){
  m_write_buffer.str("");
  m_write_buffer << "# R = "   <<m_montecarlo.R()<<"\n";
  m_write_buffer << "# J = "   <<m_montecarlo.J()<<"\n";
  m_write_buffer << "# Q"<<m_montecarlo.N1_symmetry_num()<<" = "<<m_montecarlo.QN1()<<"\n";
  m_write_buffer << "# Q"<<m_montecarlo.N2_symmetry_num()<<" = "<<m_montecarlo.QN2()<<"\n";
  m_write_buffer << "# T = "   <<m_montecarlo.T()<<"\n";
  m_write_buffer << "# pdel = "<<m_montecarlo.pdel()<<"\n";
  m_write_buffer << "# rho\trho_sq\tN1_tau\tN1_tau_sq\tN2_tau\tN2_tau_sq\n";

  WriteBufferToFile();
}


void OrderParamFile::PrepareFile(const vector<FNameOpt>& fname_include, ios::openmode write_openmode){
  if (!m_has_write_file){
    string filename;
    if (fname_include.size() == 0){
      vector<FNameOpt> default_include;
      default_include.push_back(kR);
      default_include.push_back(kJ);
      default_include.push_back(kQN1);
      default_include.push_back(kQN2);
      filename = MakeFileName(default_include);
    }
    else{
      filename = MakeFileName(fname_include);
    }
    init_write_file(filename, write_openmode);
    InsertHeader();

  }
  else{
    cout << "Write file already prepared (m_has_write_file == true). Will not re-prepare.\n"
	 << "Use the UpdateMCFile method or something that isn't written.\n"
         << "Or just fail less. That's always an option."
	 << "=> Skipping OrderParamFile::PrepareMCFile()" << endl;
  }
}


OrderParamFile::OrderParamFile(MonteCarlo& mc_save, LogFile* log_file)
  :FileHandler(), m_montecarlo(mc_save),m_log_file(log_file){
}


void OrderParamFile::Track(){
  m_write_buffer.str("");
  for (int i = 0 ; i < kNColumns ; i++){
    switch (i){
    case kRho:
      m_write_buffer<< m_montecarlo.rho() <<"\t";
      break;
    case kRhoSq:
      m_write_buffer<< m_montecarlo.rho() * m_montecarlo.rho()<<"\t";
      break;
    case kN1OP:
      m_write_buffer<< m_montecarlo.N1_OP() / 2<< "\t";
      break;
    case kN1OPSq:
      m_write_buffer<<(m_montecarlo.N1_OP() / 2)*(m_montecarlo.N1_OP() / 2)<<"\t";
      break;
    case kN2OP:
      m_write_buffer<< m_montecarlo.N2_OP() / 2<< "\t";
      break;
    case kN2OPSq:
      m_write_buffer<<(m_montecarlo.N2_OP() / 2)*(m_montecarlo.N2_OP() / 2)<<"\t";
      break;
    default:
      break;
    }
  }
  m_write_buffer << endl;

  //Write to the file and check for error output
  if (WriteBufferToFile())
    cout << "Tracked!" <<endl;
  else
    cout << "Failure to write occurred in OrderParamFile::Track" <<endl;
}


int OrderParamFile::MakeImage(FileHandler::FColumn y_axis){
  ostringstream system_request("");
  string image_write_name = write_name();

  FindIndexedName(&image_write_name, "png");
  system_request << "gp_script "
    << write_name() << " "
    << image_write_name << " "<< (y_axis+1);


  if (m_log_file != 0){
    cout << "There is a non-null LogFile pointer in the OrderParamFile!" << endl;
    cout << "Updating file name to " << image_write_name << endl;
    m_log_file->UpdateLog(image_write_name);
  }
  else{
    cout << "There ISN'T!!! IS NOT!!! a non-null LogFile pointer in the OrderParamFile!" << endl;
  }
  return system(system_request.str().c_str());
}
