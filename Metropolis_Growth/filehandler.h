//filehandler.h
/*--------------------------------------------------
  File Handler:

  This class allows for easy interfacing with files,
  using relatively safe and robust methods.

  This class is setup for polymorphism, to encourage
  users to build their own classes implementing IO
  for specific headers, data formats, and options like 
  gnuplot interfaces.
  --------------------------------------------------*/

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

#ifndef __IMAGEHANDLER_H_INCLUDED__
#define __IMAGEHANDLER_H_INCLUDED__

class 


class FileHandler{
 private:

  ostringstream m_write_name;
  ostringstream m_write_buffer;
  ios_base::openmode m_write_openmode;
  ofstream m_write_file;
  bool m_has_write_file;
  void rename_file(string& old_name, string& new_name);

 protected:
  inline explicit void set_write_name(string& write_name)
  {m_write_name = ostringstream(write_name);}
  inline explicit void set_write_openmode(ios_base::openmode write_openmode)
  {m_write_openmode = write_openmode;}
  inline string write_name(){return m_write_name;}
  inline ios_base::openmode write_openmode(){return m_write_openmode;}

 public:
  enum FNameOpt{kR, kJ,kQN1,kQN2,kT,kPDel};
  FileHandler(string& init_write_name = "");
  //Starts a new write file without migration of current file
  void init_write_file(string& init_write_name, ios_base::openmode write_openmode = ate);
  //Starts a new write file with migration of current file
  void rename_write_file(string& new_write_name);

  virtual void InsertHeader();

}


class MonteCarloFile : public FileHandler{
 public:
  MonteCarloFile();
  void InsertHeader();

  
}



#endif //__IMAGEHANDLER_H_INCLUDED__
