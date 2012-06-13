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
  enum Phase{ GAS, LIQUID, SOLID };

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
  double getPhiMacro(){return (double(orders[0]) * 2.0) - 1;}
  double getPhi(){return (double(orders[0]) * 2.0 / n_sites) - 1;}
  double getPhiSq(){return ((double(orders[0]) * 2.0) - 1)*((orders[0] * 2.0) - 1);}
  double getRho(){return double(orders[0]) / n_sites;}
  double getTau(){return double(orders[1]) / n_sites;}
  double getTauSq(){return double(orders[1])*orders[1];} // does nothing...
  double getOmega(){return double(orders[2]) / n_sites;}
  virtual Site* getSite(vector<int> coords)=0;

  void setParams(Site::pvec p_in){
    params = p_in;
    E = findInitialEnergy();
    orders[0] = findInitialRho();
    orders[1] = findInitialTau();
    orders[2] = findInitialOmega();
  }
  void setT(double T_in){T = T_in;}
                                                   
  void fileSetup();



  /*--------------------------------------------------
    Monte-carlo functions
    --------------------------------------------------*/  


  void metroMove();       //Standard metropolis MC move
  void optimize();         //Only application is running before optMetroMove()
  void optMetroMove();   //Optimized, requires call to optimize() before use
  void t_optMetroMove(){ //Tracking and optimized, requires call to optimize() before use
    optMetroMove();
    track();
  }

  void resetOP(){
    E = findInitialEnergy();
    orders[0] = findInitialRho();
    orders[1] = findInitialTau();
    orders[2] = findInitialOmega();
  }

 protected:
  
  int pullRandomSite();
  void track();

  /*
    Ideally, find_initial_energy() would be non-virtual.
    It could use pull_neighbors, and construct a visited-edge array.
    But it is not worth developing at the current time.
  */
  virtual double findInitialEnergy() =0; 
  virtual double findInitialRho() =0;
  virtual double findInitialTau() =0;
  virtual double findInitialOmega() =0;

  virtual Site::svec pullNeighbors(int site) =0;




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
 
  Site::ovec orders;   //   phi, tau, and omega, order parameters, all macroscopic and integer
  Site::pvec params;//   any number of interaction paramters
  Site::dirtable directions;
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

  SquareLattice(Site::pvec par_in,int R_in = Rdft,
                double T_in = Tdft, double pdel_in = pdeldft, MTRand* rng_in = 0,
                vector<int> sizes_in = sizedft, Phase phase_in = phasedft, Interaction itr_in = itrdef)
    : Lattice(par_in[0], par_in[1], par_in[2],R_in, T_in, pdel_in, rng_in)  
    {
      initLat(sizes_in, phase_in, itr_in);
    }

  
  void initLat(vector<int> sizes_in, Phase phase_in, Interaction itr_in);
  Site* getSite(vector<int> coords);
  void printLat();
    
 protected:
  double findInitialEnergy();
  double findInitialRho();
  double findInitialTau();
  double findInitialOmega();
  Site::svec pullNeighbors(int site);

};






#endif // __LATTICE_H_INCLUDED__




