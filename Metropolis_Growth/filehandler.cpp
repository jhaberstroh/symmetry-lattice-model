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


/*--------------------------------------------------
  Utitlity Function
  --------------------------------------------------*/
bool FileExists(const std::string& filename)
{
    struct stat buf;
    if (stat(filename.c_str(), &buf) != -1)
    {
        return true;
    }
    return false;
};


/*--------------------------------------------------
  Main code
  --------------------------------------------------*/



//Moves the contents of old_name to new_name.
//Assumes that files exist and all of that goodness.
void FileHandler::rename_file(const string& old_name, const string& new_name){
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
}


void FileHandler::rename_write_file(const string& new_write_name){
  if (m_has_write_file){
    rename_file(m_write_name.str(), new_write_name);
    m_write_name.str(new_write_name);
  }
  else{
    cout << "Cannot rename file; no initial file exists!\n"
	 << "=> Rename skipped." << endl;
  }
}


void FileHandler::init_write_file(const string& init_write_name, ios_base::openmode write_openmode){
  if (m_has_write_file)
    cout << "There is a file already prepared."
	 << "Re-initializing to a new file instead of renaming the current file." << endl;
  if (m_write_file_open)
    m_write_file.close();
  set_write_name(init_write_name);
  set_write_openmode(write_openmode);
  m_has_write_file = true;
}


FileHandler::FileHandler(const string& init_write_name, ios::openmode write_openmode)
  : m_write_name(init_write_name),m_write_openmode(write_openmode), 
    m_has_write_file(false), m_write_file_open(false){
  if (init_write_name.compare("") != 0)
    m_has_write_file = true;
}


bool FileHandler::WriteBufferToFile(){
  if (m_has_write_file){
    if (!m_write_file_open){
      m_write_file.open(m_write_name.str().c_str(), m_write_openmode);
      m_write_file_open = true;
      m_write_file << m_write_buffer.str().c_str();
      m_write_file.close();
      m_write_file_open = false;
      return true;
    }
    else{
      cout << "Minor error: Write file is already open.\n"
           << "=> Skipping FileHandler::WriteStringToFile()"<<endl;
      return false;
    }
  }
  else{
    cout << "Minor error: There is no initialized file to write to.\n"
         << "=> Skipping FileHandler::WriteStringToFile()"<<endl;
    return false;
  }
}


/*--------------------------------------------------
  MonteCarloFile Members
  --------------------------------------------------*/
string MonteCarloFile::MakeFileName(const vector<FNameOpt>& fname_include){
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
  if (FileExists(strbuf.str() + ".csv")){
    int i = 0; 
    ostringstream tempstream(strbuf.str());
    while (FileExists(tempstream.str()) && i < 1000){
      i++;
      tempstream.str(strbuf.str());
      tempstream << "_" << i << ".csv";
    }
    if (i < 100){
      strbuf << "_" << i << ".csv";
    }
    else{
      strbuf << ".csv"; 
      cout << "No available files to write into! Appending to the non-indexed file.";
    }
  }
  else{
    strbuf << ".csv"; 
  }
  return strbuf.str();
}


void MonteCarloFile::InsertHeader(){
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


void MonteCarloFile::PrepareMCFile(const vector<FNameOpt>& fname_include, ios::openmode write_openmode){
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
	 << "=> Skipping MonteCarloFile::PrepareMCFile()" << endl;
  }
}
 

MonteCarloFile::MonteCarloFile(MonteCarlo& mc_save)
  :FileHandler(), m_montecarlo(mc_save){
}


void MonteCarloFile::Track(){
  m_write_buffer.str("");  
  m_write_buffer << m_montecarlo.rho() << "\t"
                 << m_montecarlo.rho() * m_montecarlo.rho()<< "\t"
                 << m_montecarlo.N1_OP() / 2<< "\t"
                 <<(m_montecarlo.N1_OP() / 2)*(m_montecarlo.N1_OP() / 2)<< "\t"
                 << m_montecarlo.N2_OP() / 2<< "\t"
                 <<(m_montecarlo.N2_OP() / 2)*(m_montecarlo.N2_OP() / 2)<<endl;
  //Write to the file and check for error output
  if (WriteBufferToFile())
    cout << "Tracked!" <<endl;
  else
    cout << "Failure to write occurred in MonteCarloFile::Track" <<endl;    
}


void MonteCarloFile::MakeOPImage(){
  //TODO: (jhaberstroh@lbl.gov) implement
}

void MonteCarloFile::MakeLatticeImage(){
  //TODO: (jhaberstroh@lbl.gov) implement
}
