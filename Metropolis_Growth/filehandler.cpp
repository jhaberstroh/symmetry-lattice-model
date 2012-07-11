//filehandler.cpp
//TODO: (jhaberstroh@lbl.gov) include checks for existence of files
//      to improve robustness a lot.
//TODO: (jhaberstroh@lbl.gov) include a log file which tracks recently
//      written files to improve usability and access.

//File-opening code with ostringstream and ofstream:
//  const char* c_write_name = m_write_name.str().c_str();
//  m_write_file.open(c_write_name, m_write_openmode);


#include "montecarlo.h"
#include "filehandler.h"

//Moves the contents of old_name to new_name.
//Assumes that files exist and all of that goodness.
void FileHandler::rename_file(string& old_name, string& new_name){
  ofstream new_file;
  ifstream old_file;
  int buffer_size = 1024;
  char buffer[buffer_size];
  old_file.open(old_name.c_str(), ios::in);
  new_file.open(new_name.c_str(), ios::out);
  //Checks for EOF and pulls data.
  //Note that get() does not destroy delimiters
  while (!old_file.get(buffer,buffer_size)){
    new_file << buffer;
  }
  old_file.close();
  new_file.close();
  remove(old_name.c_str());
  delete buffer;
}


void FileHandler::rename_write_file(string& new_write_name){
  if (m_has_write_file){
    rename_file(m_write_name, new_write_name);
    m_write_name = new_write_name;
  }
  else{
    cout << "Cannot rename file; no initial file exists!\nRename skipped." << endl;
  }
}


void FileHandler::init_write_file(string& init_write_name, ios_base::openmode write_openmode){
  if (m_has_write_file)
    cout << "There is a file already prepared. Re-initializing to a new file instead of renaming the current file." << endl;
  set_write_name(init_write_name);
  set_write_openmode(write_openmode);
  m_has_write_file = true;
}


FileHandler::FileHandler(string& init_write_name, ios::openmode write_openmode)
  : m_write_name(init_write_name),
    m_write_openmode(write_openmode), 
    m_has_write_file(false), 
    m_write_file_open(false)
{
  if (compare(init_write_name,"") != 0)
    m_has_write_file = true;
}


/*--------------------------------------------------
  MonteCarloFile Members
  --------------------------------------------------*/
string MonteCarloFile::MakeFileName(MonteCarlo* mc_save, vector<FNameOpt>& fname_include){
  ostringstream strbuf("");
  strbuf << "ordPar";
  for (unsigned int i = 0 ; i < fname_include.size() ; i++){
    switch (fname_include[i]){
    case kR:
      strbuf << "_R"<< mc_save.R();
      break;
    case kJ:
      strbuf << "_J-"<<mc_save.J();
      break;
    case kQN1:
      strbuf << "_Q"<<mc_save.N1_symmetry_num()<<"-"<<mc_save.QN1();
      break;
    case kQN2:
      strbuf << "_Q"<<mc_save.N2_symmetry_number()<<"-"<<mc_save.QN2();
      break;      
    case kT:
      strbuf << "_T-"<<m_T;
      break;      
    case kPDel:
      strbuf << "_PDEL-"<<m_delete_probability;
      break;
    default:
      strbuf << "_na_";
      break;
    }
  }
  if (FileExists(strbuf.str() + ".csv")){
    int i = 0; 
    ostringstream tempstream = strbuf;
    while (FileExists(tempstream.str()) && i < 1000){
      i++;
      tempstream = strbuf;
      tempstream << "_" << i << ".csv";
    }
    if (i < 100)
      strbuf << "_" << i << ".csv";
    else
      strbuf << ".csv"; 
      cout << "No available files to write into! Writing into the non-indexed file."
  }
  else{
    strbuf << ".csv"; 
  }
  return strbuf.str();
}


void MonteCarloFile::InsertHeader(){
  if (m_montecarlo != 0){
    m_write_buffer.str("");
    m_write_buffer << "# R = "   <<m_montecarlo->R()<<"\n";
    m_write_buffer << "# J = "   <<m_montecarlo->J()<<"\n";
    m_write_buffer << "# Q"<<m_montecarlo->N1_symmetry_num()<<" = "<<m_montecarlo->QN1()<<"\n";
    m_write_buffer << "# Q"<<m_montecarlo->N2_symmetry_num()<<" = "<<m_montecarlo->QN2()<<"\n";
    m_write_buffer << "# T = "   <<m_montecarlo->T()<<"\n";
    m_write_buffer << "# pdel = "<<m_montecarlo->pdel()<<"\n";
    m_write_buffer << "# rho\trho_sq\tN1_tau\tN1_tau_sq\tN2_tau\tN2_tau_sq\n";
    if (m_write_file.is_open())
      m_write_file << m_write_buffer.str();
    else
      cout << "Minor error: File must be open to write to the header\n\n"
           << "Action skipped." <<endl;
    m_file_buffer.str("");
  }
  else{
    cout << "Minor error: MonteCarlo simulation must be loaded to write header.\n\n"
         << "Action skipped." << endl;

  }
}


void MonteCarloFile::PrepareMCFile(vector<FNameOpt>& fname_include, ios::openmode write_openmode){
  if (!m_has_write_file){
    string filename;
    if (fname_include.size() == 0){
      vector<FNameOpt> default_include;
      default_include.push_back(kR);
      default_include.push_back(kJ);
      default_include.push_back(kQ1);
      default_include.push_back(kQ2);
      filename = MakeFileName(m_montecarlo, default_include);
    }
    else{
      filename = MakeFileName(m_montecarlo, default_include);
    }
    init_write_file(filename, write_openmode);
    m_write_file.open(c_file_name, open);
    m_write_file_open = true;
    InsertHeader();
    m_write_file.close();
    m_write_file_open = false;
  }
  else{
    cout << "Write file already prepared, skipping PrepareMCFile().\n"
         << "Use the UpdateMCFile method or something that isn't written.\n" 
         << "Or just fail less. That's always an option."<<endl;
  }
}

 
//The only constructor for the MonteCarloFile.
//When it finishes, m_has_write_file will be false.
//In order to Track(), PrepareMCFile() must be run to change
//m_has_write_file to true.
explicit MonteCarloFile::MonteCarloFile(MonteCarlo& mc_save)
  :FileHandler(), m_montecarlo(mc_save){
}


MonteCarloFile::Track(){
  if (m_has_write_file){
    cout <<"Tracking!" <<endl;
    m_write_buffer.str("");
    
    m_write_buffer << m_montecarlo->rho() << "\t"
                   << m_montecarlo->rho() * m_montecarlo->rho()<< "\t"
                   << m_montecarlo->N1_OP() / 2<< "\t"
                   <<(m_montecarlo->N1_OP() / 2)*(m_montecarlo->N1_OP() / 2)<< "\t"
                   << m_montecarlo->N2_OP() / 2<< "\t"
                   <<(m_montecarlo->N2_OP() / 2)*(m_montecarlo->N2_OP() / 2)<<endl;
    
    m_output_file.open(m_write_name.str().c_str(), m_write_openmode);
    m_output_file << m_write_buffer.str();
    m_output_file.close();
  }
  else {
    cout << "No file to track!\n\n" 
         << "Skipping MonteCarloFile::Track()" << endl;
  }
}


void MonteCarloFile::MakeOPImage(){
  //TODO: (jhaberstroh@lbl.gov) implement
}


void MonteCarloFile::MakeLatticeImage(){
  //TODO: (jhaberstroh@lbl.gov) implement
}
