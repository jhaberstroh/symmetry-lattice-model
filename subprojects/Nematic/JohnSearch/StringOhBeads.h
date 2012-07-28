//StringOhBeads.h

#ifndef __STRINGOHBEAD_H_INCLUDED__
#define __STRINGOHBEAD_H_INCLUDED__


#include <cmath>
#include <iostream>
#include <fstream>
#include <sstream>
#include "Bead.h"


class StringOhBeads
{
enum Connection{
  PINNED_START,PINNED_END,PINNED_FULL,FREE};

public:
  StringOhBeads(int length = 20,
                double pStart = 0.01, double tStart = 0.01, double oStart = 0.01,
                double pEnd   = 0.99, double tEnd   = 0.99, double oEnd   = 0.99,
                Connection c = PINNED_START);

  ~StringOhBeads();

  void string_heun_step(double T, int R,
                        double J, double Q, double Q2);

  void print_string_pos();

 private:

  void interpolate_beads();
  void impose_arclength();


  int len;
  Bead** beads;      //array of objects
  double* g;        //interpolation position in the interval [0,1]
  double* arc_p;    //position along unit-normalized arc-length; [0,1]
  double *old_p;
  double *old_t;
  double *old_o;    //for use during interpolation
  bool *mobile;     //deciding whether a certain bead performs heun moves

};
  
#endif // __STRINGOHBEAD_H_INCLUDED__
