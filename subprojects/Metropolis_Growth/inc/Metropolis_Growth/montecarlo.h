//monteCarlo.h
/*--------------------------------------------------
  MonteCarlo is a facade pattern, intended to simplify
  the interaction between the aspects of the lattice
  and the user.

  It also serves as an adapter between the Interaction,
  the Lattice, and the FileHandler.

  Finally, it implements a visitor pattern code to perform
  the Monte Carlo moves.
  --------------------------------------------------*/


#ifndef __MONTECARLO_H_INCLUDED__
#define __MONTECARLO_H_INCLUDED__

#include <iostream>
#include <sstream>
#include <fstream>
#include "interaction.h"
#include "lattice.h"
#include "orderparamfile.h"
#include "logfile.h"

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
   Member Variables
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
  OrderParamFile* m_file_handler;
  LogFile m_log_file;
  Lattice::Phase m_initialize_phase;


 /*----------------------------------------------------
    Member Functions & constructors
    ----------------------------------------------------*/
 public:
  MonteCarlo(double J_in = Jdft, double Q_in = Qdft, double Q2_in = Q2dft,
             int R_in = Rdft, double T_in = Tdft, double pdel_in = pdeldft);
  ~MonteCarlo(){delete m_file_handler;}
//TODO: make a copy constructor and an assignment operator. Fortunately, I'm not likely to be
//      using either of those anytime soon on MonteCarlo objects!

  //TODO vv: Figure out when this function is called
  //Must be run before using OrderParamFile class; called from the constructor. (???)
  void PrepareFileHandler();
  //Standard metropolis MC move
  void DoMetropolisMove();
  //One move for each site
  void DoMetropolisSweep();
  void ResetEnergy(){/*TODO: Implement!*/};

  //Print the data from the simulation to cout.
  inline void PrintTextLattice(){  m_lattice.Print();}
  void PrintOrderParameters();

  /*--------------------------------------------------
    Accessors and Mutators
    --------------------------------------------------*/
  inline OrderParamFile& order_parameter_handler(){ if (!m_file_handler->has_write_file()) m_file_handler->PrepareFile();  return *m_file_handler;}
  inline LatticeFile& lattice_handler(){ return m_lattice.lattice_handler();}
  inline Interaction& interaction(){return m_interaction;}

  inline std::string op_image_location(){return m_log_file.ReadLog();}
  inline std::string lattice_image_location(){return "";}
  inline string PhaseStringLookup(Lattice::Phase p){return m_lattice.PhaseStringLookup(p);}
  inline string PhaseStringLookup(int p){return m_lattice.PhaseStringLookup(m_lattice.IntToPhase(p));}
  inline Lattice::Phase IntToPhase(int p){return m_lattice.IntToPhase(p);}



  void reset_default_phase(Lattice::Phase new_phase);
  //A soft-reset of R; all values stay the same (This might leave rot > R. What is the effect of that?)
  inline void reset_R(int R){m_lattice.reset_R(R);}
  inline void reset_N1_symmetry_num(int N1_symmetry_num){m_interaction.reset_N1_symmetry_number(N1_symmetry_num);}
  inline void reset_N2_symmetry_num(int N2_symmetry_num){m_interaction.reset_N2_symmetry_number(N2_symmetry_num);}
  //Order of operations matters! reset_R is a soft reset, and then the reset_default_phase does a hard reset.
  //Afterwards, the baseline for order parameters is re-computed with reset_Nx_symmetry_num.
  inline void reset_full(Lattice::Phase new_phase, int R, int N1_symmetry_num, int N2_symmetry_num){
      reset_R(R);reset_default_phase(new_phase);reset_N1_symmetry_num(N1_symmetry_num);reset_N2_symmetry_num(N2_symmetry_num);}

  inline void set_j   (double J)   {m_interaction.set_j(J);    ResetEnergy();}
  inline void set_qN1 (double Q1)  {m_interaction.set_qN1(Q1); ResetEnergy();}
  inline void set_qN2 (double Q2)  {m_interaction.set_qN2(Q2); ResetEnergy();}
  inline void set_T   (double T)   {m_T = T;}
  inline void set_pdel(double pdel){m_delete_probability = pdel;}
  inline int R(){return m_lattice.R();}
  inline int N1_symmetry_num(){return m_interaction.N1_symmetry_number();};
  inline int N2_symmetry_num(){return m_interaction.N2_symmetry_number();};
  inline double J(){return m_interaction.J();}
  inline double QN1(){return m_interaction.QN1();}
  inline double QN2(){return m_interaction.QN2();}
  inline double T(){return m_lattice.R();}
  inline double pdel(){return m_delete_probability;}

  inline double rho(){return m_interaction.rho();}
  inline double N1_OP(){return m_interaction.get_N1();}
  inline double N2_OP(){return m_interaction.get_N2();}


  inline void MakeLatticePNG(){ lattice_handler().MakeSquareLatticeColorImage(m_interaction);}
};



int TestMonteCarloCode();

#endif //__MONTECARLO_H_INCLUDED__
