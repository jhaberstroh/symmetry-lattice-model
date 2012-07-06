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

#ifndef __IMAGEHANDLER_H_INCLUDED__
#define __IMAGEHANDLER_H_INCLUDED__

class FileHandler{
 private:
  ostringstream m_file_name;
  ostringstream m_write_buffer;
  ios_base::openmode m_openmode;
  ofstream m_output_file;

 public:
  FileHandler();
  
  void ChangeFileName();

  virtual void InsertHeader();

  
  
  
}


#endif //__IMAGEHANDLER_H_INCLUDED__
