//orderparamfile.h
#ifndef __ORDERPARAMFILE_H_INCLUDED__
#define __ORDERPARAMFILE_H_INCLUDED__

#include <vector>
#include "filehandler.h"
#include "logfile.h"

class MonteCarlo;

//TODO: (jhaberstroh@lbl.gov) Include a Memento pattern for saving m_montecarlo to file.
//Note: This class has a private constructor and should only be created by
//      a MonteCarlo object!
//Furthermore, this file has no default constructor. Because it must be created
// by a MonteCarlo object, that MonteCarlo object must pass itself by reference.
//Moreover, a OrderParamFile should never be passed from one MonteCarlo object
// to another. A MonteCarlo object must create its own OrderParamFile.
class OrderParamFile : public FileHandler{
 public:
  //Friendship because of the private ctor.
  friend class MonteCarlo;
  /*--------------------------------------------------
    Member Variables
    --------------------------------------------------*/
 private:
  MonteCarlo& m_montecarlo;
  //m_log_file is owned by the MonteCarlo object which is controlling the OrderParamFile
  //As of 08/01/21: m_log_file is logging the names of the current image files
  LogFile* m_log_file;
  /*--------------------------------------------------
    Constructor
    --------------------------------------------------*/
 private:
  OrderParamFile(MonteCarlo& mc_save, LogFile* log_file = 0);
  void PrepareFile(const std::vector<FNameOpt>& fname_include = std::vector<FNameOpt>(),
		     std::ios::openmode write_openmode = std::ios::app);
  /*--------------------------------------------------
    Member Functions
    --------------------------------------------------*/
 protected:
  std::string MakeFileName(const std::vector<FNameOpt>& fname_include);

  //Requires the m_write_file to be currently open.
  void InsertHeader();


 public:
  void Track();

  //Outsources to gnuplot
  int MakeImage(FColumn y_axis = kN1OP);

  void MakeLatticeImage();
};

#endif // __ORDERPARAMFILE_H_INCLUDED__
