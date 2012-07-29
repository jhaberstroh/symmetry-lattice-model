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



//TODO: (jhaberstroh@lbl.gov) Include a Memento pattern for saving m_montecarlo to file.
//Note: This class has a private constructor and should only be created by
//      a MonteCarlo object!
//Furthermore, this file has no default constructor. Because it must be created
// by a MonteCarlo object, that MonteCarlo object must pass itself by reference.
//Moreover, a MonteCarloFile should never be passed from one MonteCarlo object
// to another. A MonteCarlo object must create its own MonteCarloFile.
class MonteCarloFile : public FileHandler{
 public:
  friend class MonteCarlo;
  /*--------------------------------------------------
    Member Variables
    --------------------------------------------------*/
 private:
  MonteCarlo& m_montecarlo;
  /*--------------------------------------------------
    Constructor
    --------------------------------------------------*/
 private:
  MonteCarloFile(MonteCarlo& mc_save);
  void PrepareMCFile(const vector<FNameOpt>& fname_include = vector<FNameOpt>(),
		     ios::openmode write_openmode = ios::app);
  /*--------------------------------------------------
    Member Functions
    --------------------------------------------------*/
 protected:
  string MakeFileName(const vector<FNameOpt>& fname_include);

  //Requires the m_write_file to be currently open.
  void InsertHeader();

 public:
  void Track();

  //Outsources to gnuplot
  int MakeOPImage(FColumn y_axis = kN1OP);

  void MakeLatticeImage();
};


class LatticeFile : public FileHandler{
 public:
  typedef vector<double> RGBVect;
  enum Color { kColorRed, kColorBlue, kColorGreen, kColorOrange, kColorGrey, kColorBlack };
  friend class Lattice;

 private:
  Lattice& m_lattice;

 public:
  void PrepareLatticeFile();

 private:
  LatticeFile(Lattice& lattice_to_save);

 public:
  //MakeSimpleFile() saves only the rot and occ data
  void MakeSimpleFile(){};

  //MakeBondFile() saves the rot and occ data, as well
  // as the number of active bonds at each site
  void MakeBondNumberFile(Interaction& lattice_interaction){};

  //MakeColorImage() uses the interactions N1 and N2 to create a gnuplot file
  void MakeSquareLatticeColorImage(Interaction& lattice_interaction)
  {cout << "Dummy function for FileHandler::MakeSquareLatticeColorImage.";}

 private:
  /*
  //ColorLookup uses two different return types: return by value (RGBVect) or return by pointer (optional).
  RGBVect ColorLookup(Color c, double* r = 0, double* g = 0, double* b = 0);
  //Hard-coded assumptions about which micro-phases should be output as which colors
  Color SelectSiteColor(int n1_bonds, int n2_bonds);
  //Draws a single site at the coordinate specified.
  // Note: Pixel_size works better as an odd number.
  void DrawSite(pngwriter& png, int pixel_size, vector<int> coord, int rot, int R, int n1_bonds, int n2_bonds);
  */
  void InsertHeader(){}

};


#endif //__IMAGEHANDLER_H_INCLUDED__
