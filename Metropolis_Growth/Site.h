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
  typedef vector<int> ovec; //Order parameters, integer counts for precision
  typedef vector<vector<int> > dirtable;


  /*--------------------------------------------------
    Gets and Sets
    --------------------------------------------------*/

  bool   getOcc() {return occ;}
  int    getRot() {return rot;}
  int    getR()   {return R;}
  int    getz()   {return z;}

  void   setRot(int rot_in){rot = rot_in;}
  void   setNeigh(svec neigh_in){nghbors = neigh_in;}

  //returns interaction energy between site and the input array
  virtual double currInteraction(Site* s, pvec params)=0;
  virtual double currInteraction(Site::svec sites, pvec params)=0;



  double attemptOcc(svec neighbors, double pdel, double T_in, pvec params, ovec* order, dirtable* directions);
  double attemptRot(svec neighbors, double T_in, pvec params, ovec* order, dirtable* directions);

  //Overloaded for optimization; these use local neighbors
  double attemptOcc(double pdel, double T, pvec params, ovec* order, dirtable* directions){
    return attemptOcc(nghbors, pdel, T, params, order, directions);}
  double attemptRot(double T, pvec params, ovec* order, dirtable* directions){
    return attemptRot(nghbors, T, params, order, directions);}

  virtual double chemPotential(double T, pvec params)=0;


 protected:
  virtual double occDE(svec sites, pvec params)=0;
  virtual double rotDE(svec sites, int r_try, pvec params)=0;
  int randRot(){return rng->randInt(R-1);}


  // Users may not manually set order params; they must create
  //  a derived class which initializes the lattice as is
  //  suitable for their needs.
  void changeOcc(){if (occ == 1) occ = 0; else occ = 1;}
  void moveRot(int plus_minus){
    if (plus_minus == 1) rot = (rot + 1)%R ; 
    if (plus_minus ==-1) rot = ((rot - 1)+R)%R;
  }
 
  void changeOcc(ovec* order, dirtable* directions);
  void moveRot(int plus_minus, ovec* order, dirtable* directions);


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
};




class NemSite : public Site{
 private:
  
  
 public:
  
 NemSite(int z_in = 0, double occ_p = 0 , MTRand* rng_in= 0, int R_in = 0)
   : Site(z_in, occ_p, rng_in, R_in){};
  
  
  
  double currInteraction(Site* s, pvec params);
  double currInteraction(Site::svec sites, pvec params);
  double occDE(Site::svec sites, pvec params);
  double rotDE(Site::svec sites, int r_try, pvec params);
  double chemPotential(double T, pvec params);
  

};

static bool abs_compare(int a, int b){
  return (abs(a) < abs(b));
}

#endif // __SITE_H_INCLUDED__
