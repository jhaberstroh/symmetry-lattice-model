//interaction.h
/* ---------------------------------------------------------------
   A class which handles all of the "dynamics" of the simulation.

   The interaction function and chemical potential are specified here,
   as well as the order parameters. The order parameters are kept
   to match the type of interaction. Order parameters must be activated
   by the specification of a Lattice.
   --------------------------------------------------------------- */
#ifndef __INTERACTION_H_INCLUDED__
#define __INTERACTION_H_INCLUDED__
#include <stdexcept>
#include <cmath>
#include "lattice.h"
#include "site.h"
using namespace std;

class not_implemented_error : runtime_error{
 public:
  not_implemented_error(const string& what_err): runtime_error(what_err){};
};



class Interaction{
 public:
  typedef vector<int> LocalBondVect;
  enum OrderParameterType{kOrderTypeOcc,kOrderTypeN1,kOrderTypeN2};
  /*----------------------------------------------------
    Variables
    ----------------------------------------------------*/
 protected:
  //The interaction strengths for non-specific interactions,
  // as well as for specific interactions of symmetry_num N1 and N2
  double m_J, m_QN1, m_QN2;

  /*The symmetry_num for the rotational interactions, converted to the "division number"
    EXAMPLE:
      R = 12, and N1 is input as 4. 
      m_N1_division will be set to 3, the distance between interacting orientations.*/
  int m_N1,m_N2;
  int m_N1_division, m_N2_division;

  //Order parameter... parameters. All stored in a macroscopic form.
  // (That is, they must be scaled down by m_lattice_being_tracked.number_of_sites().)
  //Example:
  // rho_normal = double(rho)/lat_size;
  Lattice* m_lattice_being_tracked;
  Lattice::BondVect m_N1_bond_lattice;
  Lattice::BondVect m_N2_bond_lattice;
  int m_rho; int m_order_n1; int m_order_n2;
  //Tells whether to use the bond-variable order parameter or the
  // site-variable order parameter for n1 and n2;
  static const bool kOrderN1Bond=true;
  static const bool kOrderN2Bond=true;

  /*----------------------------------------------------
    Constructors
    ----------------------------------------------------*/
 public:
  Interaction(double J = 1, double QN1 = 0, double QN2 = 0, 
              int N1=2, int N2=4, Lattice* l = 0)
    : m_J(J), m_QN1(QN1), m_QN2(QN2), 
      m_N1(N1), m_N2(N2), m_lattice_being_tracked(l){
    cout << "debug: Interaction::InitOrderParameters()"<<endl;
     InitOrderParameters();}

  /*----------------------------------------------------
    Accessors and Mutators
    ----------------------------------------------------*/
  int N1_symmetry_number(){return m_N1;}
  int N1_division() {return m_N1_division;}
  int N2_symmetry_number(){return m_N2;}
  int N2_division() {return m_N2_division;}
  void set_j  (double J) {m_J  = J;}
  void set_qN1(int Q1){m_QN1= Q1;}
  void set_qN2(int Q2){m_QN2= Q2;}
  double J(){return m_J;}
  int QN1(){return m_QN1;}
  int QN2(){return m_QN2;}

  inline double rho() 	      {return double(m_rho)/m_lattice_being_tracked->number_of_sites();}
  inline double phi()	      {return ((double(m_rho)
                                        /m_lattice_being_tracked->number_of_sites()) *2) -1;}
  inline double phi_macro()   {return  (double(m_rho) * 2) - 1;}
  inline double phi_macro_sq(){return ((double(m_rho) * 2) - 1)  *  ((double(m_rho) * 2) - 1);}
  inline double get_N1()      {return  (double(m_order_n1)
                                        /m_lattice_being_tracked->number_of_sites());}
  inline double N1_macro_sq() {return (double(m_order_n1)*double(m_order_n1));}
  inline double get_N2()      {return (double(m_order_n2)
                                       /m_lattice_being_tracked->number_of_sites());}

  inline int get_N1_bonds_at_site(Lattice::Coord& coord) {
    int n1_bonds = 0;
    if (m_lattice_being_tracked != 0){
      for (int i = 0 ; i < m_lattice_being_tracked->z() ; i++){
        //i acts as the directional variable; i looks in all directions around the coordinate
        n1_bonds += m_N1_bond_lattice[m_lattice_being_tracked->LookupBondIndex(coord, i)];
      }
    }
    return n1_bonds;
  }
  inline int get_N2_bonds_at_site(Lattice::Coord& coord) {
    int n2_bonds = 0;
    if (m_lattice_being_tracked != 0){
      for (int i = 0 ; i < m_lattice_being_tracked->z() ; i++){
        n2_bonds += m_N2_bond_lattice[m_lattice_being_tracked->LookupBondIndex(coord, i)];
      }
    }
    return n2_bonds;
  }

  /*--------------------------------------------------
    Member Functions
    --------------------------------------------------*/
  void ChangeLattice(Lattice* l){
    m_lattice_being_tracked = l;
    InitOrderParameters();
  }
  
  //For one neighbor
  double get_interaction_energy(Site* s, Site* s_neighbor, 
                                int& retn_N1_bond, int& retn_N2_bond);
  //For multiple neighbors
  double get_interaction_energy(Site* s, Lattice::NeighborVect& neighbors, 
                                LocalBondVect& new_N1_bond, LocalBondVect& new_N2_bond);
  double get_chemical_potential(Site* s, double T);
  double get_occ_energy_difference(Site* s, Lattice::NeighborVect& neighbors, 
                                   double T, LocalBondVect& new_N1_bond, LocalBondVect& new_N2_bond);
  double get_rot_energy_difference(Site* s, Lattice::NeighborVect& neighbors, 
                                   int plus_minus, LocalBondVect& new_N1_bond, LocalBondVect& new_N2_bond);

  void UpdateOrderParameters(Lattice::Coord& coord, int old_rot, LocalBondVect& new_N1_bonds, LocalBondVect& new_N2_bonds);

 private:
  int InitSpecificOP(OrderParameterType opt);
  int InitRho();
  int InitNBond(int symmetry_num, vector<int>* n_bond_lattice);
  int InitNAligned(int symmetry_num, vector<int>* n_aligned_direction);
  void InitOrderParameters();
};


int TestInteractionCode();
#endif //__INTERATCTION_H_INCLUDED__
