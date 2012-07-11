//filehandler.h
/*--------------------------------------------------
  File Handler:

  This class allows for easy interfacing with files,
  using relatively safe and robust methods.

  This class is setup for polymorphism, to encourage
  users to build their own classes implementing IO
  for specific headers, data formats, and options like 
  gnuplot interfaces.
  Furthermore, constructors should be protected/private,
  and filehandler subclasses should be friends of the
  relevant object which it is handling.
  --------------------------------------------------*/


#ifndef __FILEHANDLER_H_INCLUDED__
#define __FILEHANDLER_H_INCLUDED__

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <sys/stat.h>

//TODO: Define errors and do error handling

class MonteCarlo;

class FileHandler{
 public:
  enum FNameOpt{kR, kJ,kQN1,kQN2,kT,kPDel};
  /*--------------------------------------------------
    Member Variables
    --------------------------------------------------*/  
 private:
  ostringstream m_write_name;
  ostringstream m_write_buffer;
  ios_base::openmode m_write_openmode;
  ofstream m_write_file;
 protected:
  bool m_has_write_file;
  bool m_write_file_open;
  /*--------------------------------------------------
    Accessors and Mutators
    --------------------------------------------------*/
  inline string         write_name()                            
	{ return m_write_name;}
  inline ios::openmode  write_openmode()                            
	{ return m_write_openmode;}
  inline void  set_write_name(string& write_name)          
	{ m_write_name = ostringstream(write_name);}
  inline void  set_write_openmode(ios::openmode write_openmode)
	{ m_write_openmode = write_openmode;}
 private:
  void rename_file(string& old_name, string& new_name);
  /*--------------------------------------------------
    Constructor
    --------------------------------------------------*/
 public:
  FileHandler(string& init_write_name = const string(""), ios::openmode write_openmode = ios::out);
  //Starts a new write file without migration of current file
  void init_write_file(string& init_write_name, ios::openmode write_openmode = ios::out);
  //Starts a new write file with migration of current file
  void rename_write_file(string& new_write_name);
  /*--------------------------------------------------
    Member Functions
    --------------------------------------------------*/
 protected:
  virtual void InsertHeader() = 0;
};



//TODO: (jhaberstroh@lbl.gov) Include a Memento pattern for saving m_montecarlo to file.
//Note: This class has a private constructor and should only be created by 
//      a MonteCarlo object!
class MonteCarloFile : public FileHandler{
 public:
  friend class MonteCarlo
  /*--------------------------------------------------
    Member Variables
    --------------------------------------------------*/
 private:
  MonteCarlo& m_montecarlo;
  /*--------------------------------------------------
    Constructor
    --------------------------------------------------*/
 private:
  explicit MonteCarloFile(MonteCarlo& const mc_save); 
  void PrepareMCFile(vector<FNameOpt>& fname_include = vector<FNameOpt>(0), 
                     ios::openmode write_openmode = ios::out);
  /*--------------------------------------------------
    Member Functions
    --------------------------------------------------*/
 protected:
  string MakeFileName(MonteCarlo& mc_save, vector<FNameOpt>& fname_include);
  void InsertHeader();//Requires the m_write_file to be currently open.
 public:
  void Track();
  //Outsources to gnuplot
  void MakeOPImage();
  void MakeLatticeImage();
};


bool FileExists(const std::string& filename)
{
    struct stat buf;
    if (stat(filename.c_str(), &buf) != -1)
    {
        return true;
    }
    return false;
};

#endif //__IMAGEHANDLER_H_INCLUDED__
