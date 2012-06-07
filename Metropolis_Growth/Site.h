//Site.h
/* ------------------------------------------------------------
   The object held in container class Lattice.h.

   The type of interaction between sites can be polymorphically
   decided upon instantiation (by choosing which derived class
   will be implementing the virtual functions)

   In any case, only three interaction strengths and two 
   coordinates, one boolean and one discrete integer,
   are currently implemented.
   ------------------------------------------------------------*/


#ifndef __SITE_H_INCLUDED__
#define __SITE_H_INCLUDED__


#include <cmath>
#include <iostream>
#include <vector>
#include "MersenneTwister.h"
using namespace std;





class Site{
 public:
  Site(int z_in = 0, double occ_p = 0, MTRand* rng_in = 0, int R_in = 0);
  ~Site();
  typedef vector<Site*> svec;


  /*--------------------------------------------------
    Gets and Sets
    --------------------------------------------------*/

  bool   get_occ() {return occ;}
  int    get_rot() {return rot;}
  double get_T()   {return T;}
  int    get_R()   {return R;}
  int    get_z()   {return z;}

  void   set_rot(int rot_in){rot = rot_in;}
  void   set_T(double T_in) {T = T_in;}

  //returns interaction energy between site and the input array
  virtual double curr_interaction(Site* s, vector<int> params)=0;
  virtual double curr_interaction(Site::svec sites, vector<int> params)=0;



  double attempt_occ(Site::svec neighbors, double pdel, vector<int> params);
  double attempt_occ(Site::svec neighbors, double pdel, double T_in, vector<int> params);
  double attempt_rot(Site::svec neighbors, vector<int> params);
  double attempt_rot(Site::svec neighbors, double T_in, vector<int> params);


 protected:
  virtual double occ_dE(Site::svec sites, vector<int> params)=0;
  virtual double rot_dE(Site::svec sites, int r_try, vector<int> params)=0;
  virtual double chem_potential(vector<int> params)=0;
  int rand_rot(){return rng->randInt(R-1);}


  // Users may not manually set order params; they must create
  //  a derived class which initializes the lattice as is
  //  suitable for their needs.
  void change_occ(){if (occ == 1) occ = 0; else occ = 1;}
  void move_rot(int plus_minus){
    if (plus_minus == 1) rot = (rot + 1)%R ; 
    if (plus_minus ==-1) rot = ((rot - 1)+R)%R;
  } 


  /*--------------------------------------------------
    Variables
    --------------------------------------------------*/
  int R;         //number of directional states
  int z;         //number of nearest neighbors (for optimization)
  bool occ;
  int rot;
  MTRand* rng;
  bool rng_new;

};




class NemSite : public Site{
 private:
  
  
 public:
  
 NemSite(int z_in = 0, double occ_p = 0 , MTRand* rng_in= 0, int R_in = 0)
   : Site(z_in, occ_p, rng_in, R_in){};
  
  
  void set_interaction(double J_in, double Q_in, double Q2_in);
  
  double curr_interaction(Site* s, vector<int> params);
  double curr_interaction(Site::svec sites);
  double occ_dE(Site::svec sites);
  double rot_dE(Site::svec sites, int r_try);
  double chem_potential();
  

};



#endif // __SITE_H_INCLUDED__
