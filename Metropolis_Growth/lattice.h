//lattice.h
/* ---------------------------------------------------------------
   This is an object-oriented container class, holding an array
   of generic Site objects.

   An optimized Lattice trades space for speed; it has a vector
   of site-pointer-vectors, which allow for easy neighbor lookup.
   --------------------------------------------------------------- */
#ifndef __LATTICE_H_INCLUDED__
#define __LATTICE_H_INCLUDED__

#include <iostream>
#include <vector>
#include <stdexcept>
#include <algorithm>
#include "MersenneTwister.h"
#include "site.h"
using namespace std;

class vector_size_error : public length_error{
 public:
  int m_requested_size;
  int m_received_size;
  explicit vector_size_error(const string& what_arg, int requested_size, int received_size)
	: length_error(what_arg), m_requested_size(requested_size), m_received_size(received_size){}
};

class container_value_mismatch_error : public runtime_error{
 public:
  int m_contained_value;
  int m_contained_index;
  int m_container_value;
  explicit container_value_mismatch_error
	(const string& what_arg, int contained_value, int container_value, int contained_index)
	: runtime_error(what_arg), 
	m_contained_value(contained_value), 
	m_contained_index(contained_index),
	m_container_value(container_value){}
};

class bad_symmetry_number : public invalid_argument{
 public:
  int m_R_owned;
  int m_symmetry_number;
  explicit bad_symmetry_number
    (const string& what_arg, int R_owned, int symmetry_number)
    : invalid_argument(what_arg),
    m_R_owned(R_owned),
    m_symmetry_number(symmetry_number){}
};


class Lattice{
 public:
  typedef vector<Site*> NeighborVect; 
  typedef vector<Site*> SiteVect;  //Site vector
  enum Phase{ GAS, LIQUID, SOLID, FERRO };
  /*----------------------------------------------------
    Variables
    ----------------------------------------------------*/
 protected:
  int m_R;
  int m_z;
  //Vector of Site objects, the main functionality of the
  // Lattice container class.
  //Objects are re-initialized in constructor Lattice()
  // in the phase specified by [Phase initializing_phase].
  //(RAII safe)
  SiteVect m_lattice;                          
  //Vector of pointers to the neighbors.
  //Ownership of pointed-to Sites belongs to m_lattice.
  // (RAII safe)
  vector<NeighborVect> m_site_neighbors; 
  //Quantity specified by the derived class.
  int m_dimensionality;
  //Format specified by the derived class, usually simple
  // like {length, width}
  vector<int> m_measurements;

  /*--------------------------------------------------
    Constructor
    --------------------------------------------------*/
 public:
  Lattice();
  Lattice(int R, int z, int dimensionality, const vector<int>& sizes)
	: m_R(R), m_z(z), m_dimensionality(dimensionality), m_measurements(sizes) {};
  ~Lattice();
 Lattice(const Lattice &cSource);
 Lattice& operator=(const Lattice& cSource);

  /*--------------------------------------------------
    Accessors and Mutators
    --------------------------------------------------*/
  inline vector<int>& 	measurements()			 {return m_measurements;}
  inline int            R()                              {return m_R;}
  inline int            z()                              {return m_z;}
  inline int 		number_of_sites()		 {return m_lattice.size();}
  inline Site* 		random_site(MTRand& rng, NeighborVect** random_neighbors)	{
	int site_index = rng.randInt(number_of_sites()-1);
	(*random_neighbors) = get_neighbors(site_index);
	return m_lattice[site_index];
	}
 protected:
  inline Site*         	get_site     (int site_index)	 {return m_lattice[site_index];}
  inline NeighborVect* 	get_neighbors(int site_index)	 {return &(m_site_neighbors[site_index]);}

 public:
//calls virtual CoordToIndex function
  inline Site*         	get_site     (vector<int>& coords){return get_site     (CoordToIndex(coords));} 
  inline NeighborVect* 	get_neighbors(vector<int>& coords){return get_neighbors(CoordToIndex(coords));} 

  virtual void get_neighbors_init(int site, NeighborVect* output)=0;
  virtual void Print() = 0;
 protected:
  virtual vector<int> IndexToCoord(int index)=0;
  virtual int CoordToIndex(vector<int>& coord)=0;
 public:
  //Functions to compute the order parameters (inefficient for multiple uses) 
  //(should possibly belong to interaction.h):
  //  Returns the number of occupied sites
  virtual int ComputeNOcc() =0;
  //  Returns the number of occupied sites aligned in symmetry-generalized "mode" direction
  virtual int ComputeNAligned(int symmetry_num, vector<int>* directions) =0; 
  //  Returns the number of bonds satisfying the given symmetry (#bonds = 2x#sites)
  virtual int ComputeNBond(int symmetry_num) =0;
  //Note: symmetry_num is the total number of directions which are relevant for that symmetry
  //      i.e. symmetry_num = 2 is ferromagnetic, symmetry_num = 4 is nematic
};

int FindIndexOf(vector<Site*> array, Site* s);


  /*----------------------------------------------------
    Derived Class: SquareLattice
    ----------------------------------------------------*/

class SquareLattice : public Lattice
{
 public:
  //Constructor will throw vector_size_error if it does not receive exactly two sizes
  SquareLattice(Phase default_phase = LIQUID, const vector<int>& measurements = vector<int>(), int R = 8, MTRand* rng = 0);
  void get_neighbors_init(int site, NeighborVect* output);
  void Print();

 protected:
  vector<int> IndexToCoord(int index);  
  //CoordToIndex will throw vector_size_error if coord does not have exactly two inputs
  int CoordToIndex(vector<int>& coord);

 public:
  int ComputeNOcc();
  //ComputeNAligned will throw container_value_mismatch_error if m_R on a site does not match m_R on the lattice.
  int ComputeNAligned(int symmmetry_num, vector<int>* directions); 
  int ComputeNBond(int symmetry_num);
};

static bool abs_compare(int a, int b){
  return (abs(a) < abs(b));
}


int TestLatticeCode();



#endif //__LATTICE_H_INCLUDED__




