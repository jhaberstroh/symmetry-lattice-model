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
  Utility Function
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
    cout << "In FileHandler::init_write_file - There is a file already initialized."
	 << "Closing the old file and opening a new file with the new name." << endl;
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


bool FileHandler::WriteBufferToFile()){
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

bool FileHandler::FindIndexedName(string* file_name, const string& extension_sans_dot){
    int kMaxIndex = 10000;

    if (FileExists(*file_name + "." + extension_sans_dot)){
        int i = 0;
        ostringstream temp_fname("");
        temp_fname << *file_name << "_" << i << "." << extension_sans_dot;
        //DEBUG:
        //cout << temp_fname.str() << " and " << FileExists(temp_fname.str()) << endl;
        while (FileExists(temp_fname.str()) && i < kMaxIndex){
            i++;
            temp_fname.str("");
            temp_fname << *file_name << "_" << i << "." << extension_sans_dot;
            //DEBUG:
            //cout << temp_fname.str() << " and " << FileExists(temp_fname.str()) << endl;
        }
        if (i < kMaxIndex){
            temp_fname.str("");
            temp_fname << *file_name << "_" << i;
            *file_name = temp_fname.str();
        }
        else{
            cout << "in FileHandler::FindNexNameIndex - No available files to write into! Returning to the non-indexed file name.";
            return false;
        }
        return true;
    }
    return false;
}
