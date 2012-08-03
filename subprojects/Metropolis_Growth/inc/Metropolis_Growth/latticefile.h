//latticefile.h

#ifndef LATTICEFILE_H
#define LATTICEFILE_H

#include <vector>
#include "pngwriter.h"
#include "filehandler.h"

class Lattice;

class LatticeFile : public FileHandler{
 public:
  typedef std::vector<double> RGBVect;
  enum Color { kColorRed, kColorBlue, kColorGreen, kColorOrange, kColorGrey, kColorBlack };
  //Friendship because of the private ctor.
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
  void MakeSquareLatticeColorImage(Interaction& lattice_interaction);

 private:

  //ColorLookup uses two different return types: return by value (RGBVect) or return by pointer (optional).
  RGBVect ColorLookup(Color c, double* r = 0, double* g = 0, double* b = 0);
  //Hard-coded assumptions about which micro-phases should be output as which colors
  Color SelectSiteColor(int n1_bonds, int n2_bonds);
  //Draws a single site at the coordinate specified.
  // Note: Pixel_size works better as an odd number.
  void DrawSite(pngwriter& png, int pixel_size, std::vector<int> coord, int rot, int R, int n1_bonds, int n2_bonds);

  void InsertHeader(){}

};

#endif // LATTICEFILE_H
