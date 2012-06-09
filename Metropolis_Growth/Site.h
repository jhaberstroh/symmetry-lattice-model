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
  typedef vector<Site*> svec;  //Site vector
  typedef vector<double> pvec; //Parameter vector 
  typedef vector<double> ovec; //Order Parameter vector


  /*--------------------------------------------------
    Gets and Sets
    --------------------------------------------------*/

  bool   get_occ() {return occ;}
  int    get_rot() {return rot;}
  int    get_R()   {return R;}
  int    get_z()   {return z;}

  void   set_rot(int rot_in){rot = rot_in;}
  void   set_neigh(svec neigh_in){nghbors = neigh_in;}

  //returns interaction energy between site and the input array
  virtual double curr_interaction(Site* s, pvec params)=0;
  virtual double curr_interaction(Site::svec sites, pvec params)=0;



  double attempt_occ(svec neighbors, double pdel, double T_in, pvec params, ovec* order);
  double attempt_rot(svec neighbors, double T_in, pvec params, ovec* order);

  //uses local neighbors
  double attempt_occ(double pdel, double T, pvec params, ovec* order){
    return attempt_occ(nghbors, pdel, T, params, order);}
  double attempt_rot(double T, pvec params, ovec* order){
    return attempt_rot(nghbors, T, params, order);}



 protected:
  virtual double occ_dE(svec sites, pvec params)=0;
  virtual double rot_dE(svec sites, int r_try, pvec params)=0;
  virtual double chem_potential(double T, pvec params)=0;
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
  svec nghbors;  //A vector of neighboring sites (for optimiziation)

  bool occ;
  int rot;
  MTRand* rng;
  bool rng_new;
  static ovec dOrder;

};




class NemSite : public Site{
 private:
  
  
 public:
  
 NemSite(int z_in = 0, double occ_p = 0 , MTRand* rng_in= 0, int R_in = 0)
   : Site(z_in, occ_p, rng_in, R_in){};
  
  
  
  double curr_interaction(Site* s, pvec params);
  double curr_interaction(Site::svec sites, pvec params);
  double occ_dE(Site::svec sites, pvec params);
  double rot_dE(Site::svec sites, int r_try, pvec params);
  double chem_potential(double T, pvec params);
  

};



#endif // __SITE_H_INCLUDED__
