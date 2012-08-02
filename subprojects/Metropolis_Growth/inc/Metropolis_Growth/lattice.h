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
#include "latticefile.h"

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
  explicit container_value_mismatch_error(const string& what_arg, int contained_value, int container_value, int contained_index)
	: runtime_error(what_arg),
	  m_contained_value(contained_value),
	  m_contained_index(contained_index),
	  m_container_value(container_value)
    {}
};

class bad_symmetry_number : public invalid_argument{
 public:
  int m_R_owned;
  int m_symmetry_number;
  explicit bad_symmetry_number
    (const string& what_arg, int R_owned, int symmetry_number)
    : invalid_argument(what_arg),
      m_R_owned(R_owned),
      m_symmetry_number(symmetry_number)
    {}
};

class bad_direction : public invalid_argument{
 public:
  int m_direction_chosen;
  int m_z;
  explicit bad_direction
    (const string& what_arg, int direction_chosen, int z)
    : invalid_argument(what_arg),
      m_direction_chosen(direction_chosen),
      m_z(z)
    {}
};


class Lattice{
 public:
  typedef vector<Site*> NeighborVect;
  typedef vector<Site*> SiteVect;  //Site vector
  typedef vector<int> Coord;
  typedef vector<int> BondVect;
  enum LatticeType{kSquareLattice};
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
  LatticeFile* m_lattice_handler;

  /*--------------------------------------------------
    Constructor
    --------------------------------------------------*/
 public:
  Lattice();
  //Passes a reference to itself to the m_lattice_handler object!
  // The constructor for m_lattice_handler must do nothing but store this variable.
  Lattice(int R, int z, int dimensionality, const vector<int>& sizes)
    : m_R(R), m_z(z), m_dimensionality(dimensionality), m_measurements(sizes), m_lattice_handler(new LatticeFile(*this)){};
  ~Lattice();
 Lattice(const Lattice &cSource);
 Lattice& operator=(const Lattice& cSource);

  /*--------------------------------------------------
    Accessors and Mutators
    --------------------------------------------------*/
  inline vector<int>& 	measurements()			 {return m_measurements;}
  inline int            dimensionality()                 {return m_dimensionality;}
  inline int            R()                              {return m_R;}
  inline int            z()                              {return m_z;}
  inline int 		number_of_sites()		 {return m_lattice.size();}
  inline Site* 		random_site(MTRand& rng, NeighborVect** random_neighbors, Coord* return_coord = 0){
    int site_index = rng.randInt(number_of_sites()-1);
    (*random_neighbors) = get_neighbors(site_index);
    if (return_coord != 0)
      (*return_coord) = IndexToCoord(site_index);
    return m_lattice[site_index];
  }
 protected:
  inline Site*         	get_site     (int site_index)	 {return m_lattice[site_index];}
  inline NeighborVect* 	get_neighbors(int site_index)	 {return &(m_site_neighbors[site_index]);}

  //view_site returns a vector of rot() for a site and its neighbors.
  //It also incorporates occupancy; If a site is unoccupied,
  // it is given a sentinel value of -1.
  //This allows for traversal of the state data in the lattice without breaking encapsulation.
  vector<int>    view_site(int site_index);

 public:
  //All public functions are accessed with coordinates instead of array indexes to increase
  //the amount of encapsulation.
  //All of these are already defined because they call the VIRTUAL CoordToIndex function.
  inline Site*         	get_site     (const Coord& coords){return get_site     (CoordToIndex(coords));}
  inline NeighborVect* 	get_neighbors(const Coord& coords){return get_neighbors(CoordToIndex(coords));}
  inline vector<int>    view_site    (const Coord& coords){return view_site(CoordToIndex(coords));}

  inline LatticeFile& lattice_handler(){return *m_lattice_handler;}

  /*--------------------------------------------------
    Virtual Functions
    --------------------------------------------------*/
  virtual void Print() = 0;
 protected:
  virtual void InitializeNeighborVector(int site, NeighborVect* output)=0;
 public:
  virtual BondVect CreateBondVector()=0;
  virtual Coord IndexToCoord(int index)=0;
  virtual int   CoordToIndex(const Coord& coord)=0;

  //GetNeighborCoord works intimately with get_neighbors and view_site;
  //The ordering of the NeighborVect from get_neighbors is very specific,
  // and knowledge of which site is sharing the bond can be extracted
  // from the index.
  virtual Coord GetNeighborCoord(const Coord& coord, int direction)=0;
  virtual int LookupBondIndex(const Coord& coord, int direction)=0;
};


int FindIndexOf(vector<Site*> array, Site* s);


  /*----------------------------------------------------
    Derived Class: SquareLattice
    ----------------------------------------------------*/

class SquareLattice : public Lattice
{
 private:
  //The number of quantites in this enum must be identical to z for a SquareLattice
  //This is private because it is too messy to share with a user. The index within
  // neighbor-vector will suffice to run GetNeighborCoord and LookupBondIndex.
  enum NeighborPosition{kNeighUp, kNeighDown, kNeighLeft, kNeighRight};

 public:
  //Constructor will throw vector_size_error if it does not receive exactly two sizes
  SquareLattice(Phase default_phase = LIQUID, const vector<int>& measurements = vector<int>(), int R = 8, MTRand* rng = 0);

  void Print();
 protected:
  //Fills output[site] with the appropriate neighbors
  // using the enum values for ordering.
  //Usually acts on m_site_neighbors.
  void InitializeNeighborVector(int site, NeighborVect* output);

 public:
  //Constructs a bond vector for the current lattice.
  BondVect CreateBondVector();

  //CoordToIndex will throw vector_size_error if coord does not have exactly two inputs
  Coord IndexToCoord(int index);
  int   CoordToIndex(const Coord& coord);

  //These virtual functions are not overloaded, but rather hidden, since the inherited
  // class uses a different typing. This may be a problem but we'll see.
  Coord GetNeighborCoord(const Coord& coord, int direction);
  int LookupBondIndex(const Coord& coord, int direction);

  inline Coord GetNeighborCoord(const Coord& coord, NeighborPosition direction){
    return GetNeighborCoord(coord, (int)direction);
  }
  inline int LookupBondIndex(const Coord& coord, NeighborPosition direction){
    return LookupBondIndex(coord, (int)direction);
  }

};

static bool abs_compare(int a, int b){
  return (abs(a) < abs(b));
}


int TestLatticeCode();



#endif //__LATTICE_H_INCLUDED__




