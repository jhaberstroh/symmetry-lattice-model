//filehandler.cpp
#include "montecarlo.h"
#include "filehandler.h"
//TODO: (jhaberstroh@lbl.gov) include checks for existence of files
//      to improve robustness a lot.

//File-opening code with ostringstream and ofstream:
//  const char* c_write_name = m_write_name.str().c_str();
//  m_write_file.open(c_write_name, m_write_openmode);


//Assumes that files exist and all of that goodness
void FileHandler::rename_file(string& old_name, string& new_name){
  //setup for writing
  ofstream new_file;
  //setup for reading
  ifstream old_file;
  
  int buffer_size = 1024;
  char buffer[buffer_size];


  old_file.open(old_name.c_str(), ios::in);
  new_file.open(new_name.c_str(), ios::out);
  while (!old_file.get(buffer,buffer_size)){
    new_file << buffer;
  }
  old_file.close();
  new_file.close();
  remove(old_name.c_str());
  
  delete buffer;
}


FileHandler::FileHandler(string& init_write_name)
  : m_write_name(init_write_name),m_write_openmode(ios_base::ate){
  if (compare(init_write_name,"") == 0)
    m_has_write_file = false;
  else 
    m_has_write_file = true;
}

//For write_openmode, some choices of interest follow:
//  ios_base::app -> all writes will go to the end of the file
//  ios_base::ate -> opens initially at end of the file
//  ios_base::trunc -> deletes everything in the file and opens at the beginning
void FileHandler::init_write_file(string& init_write_name, ios_base::openmode write_openmode = ios_base::ate){
  if (m_has_write_file)
    cout << "File already selected; moving to a new file instead of renaming the current file" << endl;

  set_write_name(init_write_name);
  set_write_openmode(write_openmode);
  m_has_write_file = true;
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



MonteCarloFile::MonteCarloFile(vector<FNameOpt>& fname_include, MonteCarlo& mc_save, ios::openmode write_openmode){
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
      strbuf << "_PD-"<<m_delete_probability;
      break;
    default:
      strbuf << "_na_";
      break;
    }

  }
}


void MonteCarlo::SetupTrack(vector<FNameOpt> fname_include, ios_base::openmode open){
  m_file_name.str("");
  m_file_name << "ordPar_R"<<m_lattice.R();
  for (unsigned int i = 0 ; i < fname_include.size() ; i++){
    switch (fname_include[i]){
    case kJ:
      m_file_name << "_J-"<<mc_save.J();
      break;
    case kQN1:
      m_file_name << "_Q"<<m_interaction.N1_symmetry_number()<<"-"<<m_interaction.QN1();
      break;
    case kQN2:
      m_file_name << "_Q"<<m_interaction.N2_symmetry_number()<<"-"<<m_interaction.QN2();
      break;      
    case kT:
      m_file_name << "_T-"<<m_T;
      break;      
    case kPDel:
      m_file_name << "_PD-"<<m_delete_probability;
      break;
    default:
      m_file_name << "_na_";
      break;
    }
  }
  //Comma-separated value list
  m_file_name << ".csv"; 

  while (m_output_file.is_open()){
    m_output_file.flush();
    m_output_file.close();
  }
  if (!m_output_file.is_open()){
    //open the file with the selected ios_base::openmode
    //Some choices of interest follow:
    //  ios_base::app -> all writes will go to the end of the file
    //  ios_base::ate -> opens initially at end of the file
    //  ios_base::trunc -> deletes everything in the file and opens at the beginning
    const char* c_file_name = m_file_name.str().c_str();
    m_output_file.open(c_file_name, open);
    FileHeader();
    m_output_file.close();
    m_openmode = open;
  }
}
