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

  All classes which are treated by filehandler are to
  be forward declared in filehandler.h, and the .h
  files are to be included in filehandler.cpp. This
  should avoid circular dependency errors in compilation.
  (Take Lattice and MonteCarlo for example).
  --------------------------------------------------*/


#ifndef __FILEHANDLER_H_INCLUDED__
#define __FILEHANDLER_H_INCLUDED__

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <cmath>
//#include <pngwriter.h>
#include "version.h"

//TODO: Define errors and do error handling


class MonteCarlo;
class Interaction;
class Lattice;

class FileHandler{
 public:
  enum FNameOpt{kR, kJ,kQN1,kQN2,kT,kPDel};
  enum FColumn{kRho, kRhoSq,kN1OP,kN1OPSq,kN2OP,kN2OPSq,kNColumns};
  /*--------------------------------------------------
    Member Variables
    --------------------------------------------------*/
 private:
  ostringstream m_write_name;
  ios_base::openmode m_write_openmode;
  ofstream m_write_file;
 protected:
  ostringstream m_write_buffer;
  bool m_has_write_file;
  bool m_write_file_open;
  string m_filelog_location;

  /*--------------------------------------------------
    Accessors and Mutators
    --------------------------------------------------*/
 public:
  inline string         write_name(){
    return m_write_name.str();
  }
  inline const char*        c_write_name(){
    return m_write_name.str().c_str();
  }
  inline bool has_write_file(){
    return m_has_write_file;
  }
  inline ios::openmode write_openmode() {
    return m_write_openmode;
  }

 protected:
  inline void  set_write_name(const string& write_name){
    m_write_name.str(write_name);
  }
  inline void  set_write_openmode(ios::openmode write_openmode){
    m_write_openmode = write_openmode;
  }

 private:
  void rename_file(const string& old_name, const string& new_name);
  //Modifies file_name to be the next available file
  //Returns true if file_name is changed, and false otherwise.
  /*--------------------------------------------------
    Constructor
    --------------------------------------------------*/
 public:
  FileHandler(const string& init_write_name = "", ios::openmode write_openmode = ios::app);
  //Starts a new write file without migration of current file
  void init_write_file(const string& init_write_name, ios::openmode write_openmode = ios::app);
  //Starts a new write file with migration of current file
  void rename_write_file(const string& new_write_name);
  /*--------------------------------------------------
    Member Functions
    --------------------------------------------------*/
 protected:
    bool FindIndexedName(string* file_name, const string& extension_sans_dot);
    bool WriteBufferToFile();
    virtual void InsertHeader() = 0;
};




#endif //__FILEHANDLER_H_INCLUDED__
