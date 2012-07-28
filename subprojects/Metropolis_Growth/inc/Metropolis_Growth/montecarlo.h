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
#include "filehandler.h"

static double Jdft = 1;
static double Qdft = .233;
static double Q2dft = .0777;
static double Rdft = 8;
static double Tdft = 1;
static double pdeldft = .5;

class MonteCarloFile;


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
  MonteCarloFile* m_file_handler;
  

 /*----------------------------------------------------
    Member Functions & constructors
    ----------------------------------------------------*/
 public:
  MonteCarlo(double J_in = Jdft, double Q_in = Qdft, double Q2_in = Q2dft, 
             int R_in = Rdft, double T_in = Tdft, double pdel_in = pdeldft);
  ~MonteCarlo(){delete m_file_handler;}
//TODO: make a copy constructor and an assignment operator. Fortunately, I'm not likely to be
//      using either of those anytime soon on MonteCarlo objects!

  void PrepareFileHandler();
  //Standard metropolis MC move
  void DoMetropolisMove();
  //One move for each site
  void DoMetropolisSweep();
  void ResetEnergy(){/*TODO: Implement!*/};
  inline void reset_default_phase(Lattice::Phase new_phase){
    m_lattice = SquareLattice(new_phase, m_lattice.measurements(), m_lattice.R(), &m_rng);
    m_interaction.ChangeLattice(&m_lattice);
    ResetEnergy();
  };

  inline void PrintLattice(){  m_lattice.Print();}
  void PrintOrderParameters();

  /*--------------------------------------------------
    Accessors and Mutators
    --------------------------------------------------*/
  inline MonteCarloFile& order_parameter_handler(){
    if (!m_file_handler->has_write_file())
      m_file_handler->PrepareMCFile();  
    return *m_file_handler;
  }
  inline LatticeFile& lattice_handler(){
    return m_lattice.lattice_handler();
  }
  inline Interaction& interaction(){return m_interaction;}

  inline void set_j  (double J)  {m_interaction.set_j(J);    ResetEnergy();}
  inline void set_qN1(double Q1) {m_interaction.set_qN1(Q1); ResetEnergy();}
  inline void set_qN2(double Q2) {m_interaction.set_qN2(Q2); ResetEnergy();}
  inline void set_T  (double T)  {m_T = T;}
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

};



int TestMonteCarloCode();

#endif //__MONTECARLO_H_INCLUDED__
