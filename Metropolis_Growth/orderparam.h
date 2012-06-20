#include <iostream>
#include "lattice.h"


class orderparam{
 public:
  enum ptype{OCC,N1,N2};
  //OCC is the occupation order parameter
  //N1 and N2 are placeholders for order parameters with n-fold symmetry.
  
  //default constructor has nothing to do
  //Note: Constructors cannot call virtual functions.
  orderparam(){};


  virtual void initOP(Lattice const& l) = 0;
  virtual void updateOP(ptype op, vector<int> mod) = 0;
  /*Example for updateOP:
     dat = vector{1, 2};
     updateOP(N1, dat)

     **This would mean that N1 has become occupied (from mod[0])
       and that N1 has two neighbors with favorable N1 bonds (from mod[1])
  
     In general, this could be setup in any number of ways; however,
     for a given order parameter, care should be taken to follow an
     identical format amongst different OP choices.*/

  double getRho(){return   double(rho)/latSize;}
  double getPhi(){return ((double(rho)/latSize) *2) -1;}
  double getN1(){return double(n1)/latSize;}
  double getN2(){return double(n2)/latSize;}
  
 protected:
  int latSize;
  int rho;
  int n1;
  int n2;
}



class deltaDir{
  //The same order parameters from mean field theory:
  //  Select the most ordered direction (positive or negative)
  //  and use that as the value for the order parameter.
  //NOTE: Not ideal when direction is only a local trait; 
  //  i.e. if there are patches with different orientations
  //  (they will not all be included!)

  //NOTE: This could be remedied by summing the magnitudes of
  //  each direction... compare this to "bond" order parameter?
  //  (it has the benefit of being bounded between 0 and 1 like
  //   the mean field theory)
  //  (however, it still has the problem of opposing clusters
  //   inappropriately detracting from the order parameter)
 public:
 deltaDir(Lattice const& l, int Rn1_in = 2, int Rn2_in = 4)
   : n1(Rn1_in), n2(Rn2_in)
  {
    initLattice(l);
  }

  void initOP(Lattice const& l);
  void updateOP(ptype op, vector<int> mod);

 private:
  vector<int> n1Tally;
  vector<int> n2Tally;
}



class bondCount{
  //Percentage of bonds which are energetically favorable.
  //These order parameters go from -1 to 1.
 public:
  bondCount(int Rn1_in = 2, int Rn2_in = 4);
  
  void initOP(Lattice const& l);
  void updateOP(ptype op, vector<int> mod);

}

