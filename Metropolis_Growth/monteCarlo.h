//monteCarlo.h

#ifndef __MONTECARLO_H_INCLUDED__
#define __MONTECARLO_H_INCLUDED__

#include <fstream>
#include <sstream>
#include "interaction.h"
#include "lattice.h"
#include "site.h"

static double Jdft = 1;
static double Qdft = .233;
static double Q2dft = .0777;
static double Rdft = 8;
static double Tdft = 1;
static double pdeldft = .5;

class MonteCarlo{
 /*----------------------------------------------------
    VARIABLES
    ----------------------------------------------------*/
 private:


  Interaction m_interaction;
  SquareLattice m_lattice;
  //The probability of performing a delete move vs. a 
  // rotation move on an occupied site;
  // this has an influence on the probability function
  // called by DoMetropolisMove() in order to maintain
  // the detailed balance.
  double m_delete_probability;
  double m_T;
  MTRand m_rng;
  //For file handling, the file name and file pointer.
  //For no real good reason, an ostringstream is used,
  // perhaps because it is nicer to perform actions on,
  // or perhaps because some other file used one.
  ostringstream m_filename;
  FILE* m_pFile;

 /*----------------------------------------------------
    Member Functions & constructors
    ----------------------------------------------------*/
 public:
  MonteCarlo(double J_in = Jdft, double Q_in = Qdft, double Q2_in = Q2dft, 
             int R_in = Rdft, double T_in = Tdft, double pdel_in = pdeldft);
  //Standard metropolis MC move
  void DoMetropolisMove();
  void DoMetropolisSweep();


  /*--------------------------------------------------
    Accessors and Mutators
    --------------------------------------------------*/
  inline void set_j  (double J)  {m_interaction.set_j(J);    ResetEnergy();}
  inline void set_qN1(double Q1) {m_interaction.set_qN1(Q1); ResetEnergy();}
  inline void set_qN2(double Q2) {m_interaction.set_qN2(Q2); ResetEnergy();}
  inline void set_T  (double T)  {m_T = T;}
  

  //Metropolis MC move with tracking
  inline void DoTrackedMetropolisMove(){DoMetropolisMove();Track();}
  void ResetEnergy(){};
  void ResetOP(){};
  void Track();
  inline void PrintLattice(){  m_lattice.Print();}

  void TestNeighborPointers();
};



int TestMonteCarloCode();

#endif //__MONTECARLO_H_INCLUDED__
