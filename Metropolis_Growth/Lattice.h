//Lattice.h
/* ---------------------------------------------------------------
   This is an object-oriented container class, holding an array
   of generic Site objects.

   The Site objects implement the interactions when appropriately
   instantiated.

   There are a number of virtual functions; the Lattice class
   employs polymorphism extensively to allow identical analysis
   on a number of different lattice types (including in different
   dimensions!)
   --------------------------------------------------------------- */

#ifndef __LATTICE_H_INCLUDED__
#define __LATTICE_H_INCLUDED__


#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>
#include "Site.h"
#include "MersenneTwister.h"

using namespace std;

class Lattice{
 public:
  enum Interaction{   NEMATIC  };
  enum Phase{ GAS, LIQUID  };

  /*--------------------------------------------------
    Default Values
    --------------------------------------------------*/

  static const double Jdft;
  static const double Qdft;
  static const double Q2dft;
  static const double Tdft;
  static const double pdeldft;
  static const int Rdft;
  static const vector<int> sizedft;
  static const Phase phasedft;
  static const Interaction itrdef;
  
  /*--------------------------------------------------
    Basic class operations
    --------------------------------------------------*/

  Lattice(double J_in = Jdft, double Q_in = Qdft, double Q2_in = Q2dft, int R_in = Rdft,
         double T_in = Tdft, double pdel_in = pdeldft, MTRand* rng_in = 0);
  ~Lattice();
  virtual void initLat(vector<int> sizes_in, Phase phase_in, Interaction itr_in)=0;
  virtual void printLat() = 0;

  double getE(){return E/n_sites;}
  double get_phi(){return (orders[0] * 2.0 / n_sites) - 1;}
  double get_rho(){return orders[0] / n_sites;}
  double get_tau(){return orders[1] / n_sites;}
  double get_omega(){return orders[2] / n_sites;}
  virtual Site* get_site(vector<int> coords)=0;

  void set_params(Site::pvec p_in){params = p_in;}
  void set_T(double T_in){T = T_in;}
                                                   
  void file_setup();



  /*--------------------------------------------------
    Monte-carlo functions
    --------------------------------------------------*/  


  void metro_move();       //Standard metropolis MC move
  void optimize();         //Only application is running before opt_metro_move()
  void opt_metro_move();   //Optimized, requires call to optimize() before use
  void t_opt_metro_move(){ //Tracking and optimized, requires call to optimize() before use
    opt_metro_move();
    track();
  }


 protected:
  
  int pull_random_site();
  void track();

  /*
    Ideally, find_initial_energy() would be non-virtual.
    It could use pull_neighbors, and construct a visited-edge array.
    But it is not worth developing at the current time.
  */
  virtual double find_initial_energy() =0; 
  virtual double find_initial_rho() =0;
  virtual double find_initial_tau() =0;
  virtual double find_initial_omega() =0;

  virtual Site::svec pull_neighbors(int site) =0;




  /*----------------------------------------------------
    VARIABLES
    ----------------------------------------------------*/



  //The array:
  Site::svec sites;
  int n_sites;
  // # of dimensions
  int dim;
  // size of dimensions
  vector<int> sizes; 

  MTRand* rng;      //   The random number generator, and whether it should be free'd
  bool new_rng;
  double E;
 
  Site::ovec orders;   //   phi, tau, and omega, order parameters
  Site::pvec params;//   any number of interaction paramters
  int R;
  double T;
  double pdel;      //   selects how many rotation moves to growth moves are used.
  ostringstream filename;
  FILE *pFile;
};









  /*----------------------------------------------------
    Derived Class: SquareLattice
    ----------------------------------------------------*/

class SquareLattice: public Lattice
{
 public:
  
 SquareLattice(double J_in = Jdft, double Q_in = Qdft, double Q2_in = Q2dft, int R_in = Rdft,
               double T_in = Tdft, double pdel_in = pdeldft, MTRand* rng_in = 0,
               vector<int> sizes_in = sizedft, Phase phase_in = phasedft, Interaction itr_in = itrdef)           
   : Lattice(J_in, Q_in, Q2_in, R_in, T_in, pdel_in, rng_in)  
    {
      initLat(sizes_in, phase_in, itr_in);
    }
  
  void initLat(vector<int> sizes_in, Phase phase_in, Interaction itr_in);
  Site* get_site(vector<int> coords);
  void printLat();
    
 protected:
  double find_initial_energy();
  double find_initial_rho();
  double find_initial_tau();
  double find_initial_omega();
  Site::svec pull_neighbors(int site);

};




#endif // __LATTICE_H_INCLUDED__




