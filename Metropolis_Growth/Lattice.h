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
#include <vector>
#include <cmath>
#include "Site.h"
#include "MersenneTwister.h"

using namespace std;

class Lattice{
 public:

  enum Interaction{
    NEMATIC
      };

  enum Phase{
    GAS, LIQUID
      };

//----Default values----
  static const double Jdft;
  static const double Qdft;
  static const double Q2dft;
  static const double Tdft;
  static const double pdeldft;
  static const int Rdft;
  static const vector<int> sizedft;
  static const Phase phasedft;
  static const Interaction itrdef;
  
  Lattice(double J_in = Jdft, double Q_in = Qdft, double Q2_in = Q2dft, int R_in = Rdft,
         double T_in = Tdft, double pdel_in = pdeldft, MTRand* rng_in = 0);
  ~Lattice();

  
  virtual void initLat(vector<int> sizes_in, Phase phase_in, Interaction itr_in)=0;
  virtual Site* get_site(vector<int> coords)=0;
  void metro_move();
  virtual void printLat() = 0;

  double getE(){return E/n_sites;}
  double get_phi(){p = find_initial_phi(); return p;}
  double get_tau(){t = find_initial_tau(); return t;}
  double get_omega(){o = find_initial_omega(); return o;}


 protected:
  
  int pull_random_site();
  /*
    Ideally, find_initial_energy() would be non-virtual.
    It could use pull_neighbors, and construct a visited-edge array.
    But it is not worth developing at the current time.
  */
  virtual double find_initial_energy() =0; 
  virtual double find_initial_phi() =0;
  virtual double find_initial_tau() =0;
  virtual double find_initial_omega() =0;

  virtual Site::svec pull_neighbors(int site) =0;


  /*----------------------------------------------------
    VARIABLES
    ----------------------------------------------------*/
  double E;
  double p, t, o; //phi, tau, and omega

  Site::svec sites;
  int n_sites;
  vector<int> sizes; //array of proportions of each dimension (x,y,z,etc)
  int dim;

  MTRand* rng;
  bool new_rng;

  double J, Q, Q2;
  int R;
  double T;

  double pdel; //this term selects how many rotation moves to growth moves are used.
};



class SquareLattice: public Lattice
{
 public:
  
 SquareLattice(double J_in = Jdft, double Q_in = Qdft, double Q2_in = Q2dft, int R_in = Rdft,
               double T_in = Tdft, double pdel_in = pdeldft, MTRand* rng_in = 0,
               vector<int> sizes_in = sizedft, Phase phase_in = phasedft, Interaction itr_in = itrdef)           
   : Lattice(J_in, Q_in, Q2_in, R_in, T_in, pdel_in, rng_in)  
    {
      initLat(sizes_in, phase_in, itr_in);
      E = find_initial_energy();
      p = find_initial_phi();
      t = find_initial_tau();
      o = find_initial_omega();
    }
  
  void initLat(vector<int> sizes_in, Phase phase_in, Interaction itr_in);
  Site* get_site(vector<int> coords);
  void printLat();
    
 protected:
  double find_initial_energy();
  double find_initial_phi();
  double find_initial_tau();
  double find_initial_omega();
  Site::svec pull_neighbors(int site);

};




#endif // __LATTICE_H_INCLUDED__




