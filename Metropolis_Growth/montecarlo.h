//monteCarlo.h

#ifndef __MONTECARLO_H_INCLUDED__
#define __MONTECARLO_H_INCLUDED__

#include <iostream>
#include <sstream>
#include <fstream>
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
 public:
  enum FNameOpt{kJ,kQN1,kQN2,kT,kPDel};


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
  ostringstream m_file_name;
  ostringstream m_file_buffer;
  ios_base::openmode m_openmode;
  ofstream m_output_file;
  

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
  void ResetEnergy(){/*TODO: Implement!*/};
  void ResetOP(){ m_interaction.InitOrderParameters(&m_lattice);}
  inline void reset_default_phase(Lattice::Phase new_phase){
    m_lattice = SquareLattice(new_phase, m_lattice.measurements(), m_lattice.R(), &m_rng);
    ResetOP();
    ResetEnergy();
  };
  void SetupTrack(vector<FNameOpt> fname_include, ios_base::openmode open = ios_base::app);
  void SetupTrack();
  void FileHeader();
  void Track();

  inline void PrintLattice(){  m_lattice.Print();}
  inline void PrintOrderParameters(){
    cout << "Rho:   "<<m_interaction.rho()   <<"\t"<<endl;
    cout << "Tau:   "<<m_interaction.get_N1()<<"\t"
         <<m_interaction.N1_symmetry_number()<<"\t"
         <<m_interaction.N1_division()<<endl;
    cout << "Omega: "<<m_interaction.get_N2()<<"\t"
         <<m_interaction.N2_symmetry_number()<<"\t"
         <<m_interaction.N2_division()<<endl;
  }

  void TestNeighborPointers();
};



int TestMonteCarloCode();

#endif //__MONTECARLO_H_INCLUDED__
