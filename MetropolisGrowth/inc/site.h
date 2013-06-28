//site.h
/* ------------------------------------------------------------
   The object held in container class Lattice.h.

   Does very few actions; can be instantiated with different
   probabilities for orientatation, but after instantiation,
   can only be moved stochastically.
   ------------------------------------------------------------*/
#ifndef __SITE_H_INCLUDED__
#define __SITE_H_INCLUDED__
#include <iostream>
#include <vector>
#include "MersenneTwister.h"
using namespace std;

class Lattice;

class Site{
 protected:
  /*--------------------------------------------------
    Variables
    --------------------------------------------------*/
  //Number of directional states
  int  m_R;
  //Current alignment direction
  int  m_rot;
  //Current occupational state
  bool m_occ;
  /*--------------------------------------------------
    Constructor
    --------------------------------------------------*/
 public:
  friend class Lattice;
  Site(int R_in = 0, double occ_probability = 0, int rot_preference = 0, double rot_probability = 0, MTRand* rng = 0);
  /*--------------------------------------------------
    Accessors (public) & Mutators (private)
    --------------------------------------------------*/
  inline int    R()   {return m_R;}
  inline int    rot() {return m_rot;}
  inline bool   occ() {return m_occ;}

 protected:
  inline void set_R(int R) { m_R = R;}
  inline int set_rot(int rot_in){
    return m_rot = ((rot_in % m_R) +m_R) %m_R;}
  inline void flip_occ(){
    if (m_occ == 1) m_occ = 0; else m_occ = 1;}
  inline void move_rot(int plus_minus){
    if (plus_minus == 1 || plus_minus == -1) m_rot = (m_rot + plus_minus + m_R) %m_R;
    else cout<<"in Site::move_rot, bad input for move_rot!\n";}
  /*--------------------------------------------------
    Member Functions
    --------------------------------------------------*/
 public:
  inline int RandRot(MTRand& rng){
	if (!occ()) return set_rot(rng.randInt(m_R-1)); else return m_rot;}
  inline bool AttemptOcc(double prob, MTRand& rng){
	if (rng.rand() < prob){ flip_occ(); return true;} return false;}
  inline bool AttemptRot(int plus_minus, double prob, MTRand& rng){
	if (rng.rand() < prob){ move_rot(plus_minus); return true;} return false;}
};


int TestSiteCode();


 //Site vector

//if (plus_minus == 1) rot = (rot + 1)%m_R ;
//   else if (plus_minus ==-1) rot = ((rot - 1)+m_R)%m_R;
//   else cout<<"Bad input into Site::moveRot; only accepts +/-1, was given "<<plus_minus<<"."<<endl;
// }
#endif //__SITE_H_INCLUDED__
