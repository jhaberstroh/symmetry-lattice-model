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
     InitOrderParameters();}
  void InitOrderParameters(Lattice* l = 0);

  /*----------------------------------------------------
    Accessors and Mutators
    ----------------------------------------------------*/
  int N1_symmetry_number(){return m_N1;}
  int N1_division() {return m_N1_division;}
  int N2_symmetry_number(){return m_N2;}
  int N2_division() {return m_N2_division;}
  void set_j  (double J) {m_J  = J;}
  void set_qN1(double Q1){m_QN1= Q1;}
  void set_qN2(double Q2){m_QN2= Q2;}
  double J(){return m_J;}
  double QN1(){return m_QN1;}
  double QN2(){return m_QN2;}
  double get_interaction_energy(Site* s, Site* s_neighbor, 
                                int& retn_N1_bond, int& retn_N2_bond);
  double get_interaction_energy(Site* s, Lattice::NeighborVect neighbors, 
                                int& retn_N1_bond, int& retn_N2_bond);
  double get_chemical_potential(Site* s, double T);
  double get_occ_energy_difference(Site* s, Lattice::NeighborVect neighbors, 
                                   double T, vector<int>* delta_bonds);
  double get_rot_energy_difference(Site* s, Lattice::NeighborVect neighbors, 
                                   int plus_minus, vector<int>* delta_bonds); 

   /*Example for UpdateOrderParameters:
     dat = vector<int>(); dat.push_back(1); dat.push_back(2);
     updateOP(N1, dat)

     **This would mean that N1 has become occupied (from mod[0])
       and that N1 has two neighbors with favorable N1 bonds (from mod[1])
  
     In general, this could be setup in any number of ways; however,
     for a given order parameter, care should be taken to follow an
     identical format amongst different OP choices.*/  
  void update_order_parameters(OrderParameterType op, vector<int>& opts);
  
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
  

};


int TestInteractionCode();
#endif //__INTERATCTION_H_INCLUDED__
