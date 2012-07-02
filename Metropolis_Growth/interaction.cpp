//interaction.cpp
#include "interaction.h"

/*--------------------------------------------------
  Order Parameter functions
  --------------------------------------------------*/

void Interaction::update_order_parameters(OrderParameterType op, vector<int>& mod){
  //      vector<int> mod;
  //      mod.push_back(dOcc); mod.push_back(rot);
  //       mod.push_back(plus_minus); mod.push_back(dN1bond);
  //      order->updateOP(orderparam::N1, mod);
  switch (op){
  case kOrderTypeOcc:
    m_rho += mod[0];
    break;
  case kOrderTypeN1:
    //does not currently handle site-varaible order parameters!
    m_order_n1 += mod[3];
    break;
  case kOrderTypeN2:
    //does not currently handle site-varaible order parameters!
    m_order_n2 += mod[3];
    break;
  }
}

void Interaction::InitOrderParameters(Lattice* l){
  if (l != 0){
    m_lattice_being_tracked = l;
  }
  if (m_lattice_being_tracked != 0){
    m_rho =      m_lattice_being_tracked->ComputeNOcc();
    //TODO: Needs a "direction" variable to be used
    //      m_order_n1 = m_lattice_being_tracked->ComputeNAligned(m_N1);
    if (kOrderN1Bond == false)
      throw not_implemented_error("Site-based directional order parameters");
    if (kOrderN1Bond == true)
      m_order_n1 = m_lattice_being_tracked->ComputeNBond(m_N1);
    if (kOrderN2Bond == false)
      throw not_implemented_error("Site-based directional order parameters");
    if (kOrderN2Bond == true)
      m_order_n2 = m_lattice_being_tracked->ComputeNBond(m_N2);
  }
}

/*--------------------------------------------------
  Energetic functions
  --------------------------------------------------*/

//--------------------------------------------------
//Interaction of single sites
double Interaction::get_interaction_energy(Site* s, Site* s_neighbor, 
                                           int& retn_N1_bond, int& retn_N2_bond){
  //TODO: (jhaberstroh@lbl.gov) optimize the m_N1_division calculation
  m_N1_division = s->R() / m_N1;
  m_N2_division = s->R() / m_N2;
  double energy = 0;
  int add_amt = 0;
  if (s->occ()==1 && s_neighbor->occ()==1){
    energy += m_J;
    if ((s->rot() % m_N1_division) == (s_neighbor->rot() %m_N1_division)){
      add_amt = (( ( (   (s->rot()          /m_N1_division) %2) 
                     == ((s_neighbor->rot() /m_N1_division) %2) ) *2) -1); 
      retn_N1_bond += add_amt;
      energy +=  m_QN1 * add_amt;
    }
    if ((s->rot() % m_N2_division) == (s_neighbor->rot() %m_N2_division)){
      add_amt = (( ( (   (s->rot()          /m_N2_division) %2) 
                     == ((s_neighbor->rot() /m_N2_division) %2) ) *2) -1); 
      retn_N2_bond += add_amt;
      energy +=   m_QN2 * add_amt;
    }
    return -energy;
  }
  else{
    return 0;
  }
}


//--------------------------------------------------
//Interaction of a single site with a vector of neighbors
//  Used in get_occ_energy_difference && get_rot_energy_difference
double Interaction::get_interaction_energy(Site* s, Lattice::NeighborVect neighbors, 
                                           int& retn_N1_bond, int& retn_N2_bond){
  //TODO: (jhaberstroh@lbl.gov) optimize for redundant accesses on s.
  double energy = 0;
  retn_N1_bond = 0;
  retn_N2_bond = 0;
  for (unsigned int i = 0; i < neighbors.size(); i++){
    energy += get_interaction_energy(s, neighbors[i], retn_N1_bond, retn_N2_bond);
  }

  return energy;
}


//--------------------------------------------------
//Energy difference for flipping the occupation of a center site
double Interaction::get_occ_energy_difference(Site* s, Lattice::NeighborVect neighbors, 
                                              double T, vector<int>* delta_bonds){
  double dE = 0;
  int N1 = 0;
  int N2 = 0;
  //Energy of unoccupied is always 0, we have easier computations!
  if (s->occ()){
    // (dE_insert = (dU_insert = 0 - U) - mu)
    dE = -get_interaction_energy(s, neighbors, N1, N2) + get_chemical_potential(s, T);
    (*delta_bonds)[0] = -N1; 
    (*delta_bonds)[1] = -N2;
    return dE;
  }
  else{
    // (dE_insert = (dU_insert = U - 0) - mu)
    //Create a dummy occupied state
    Site temp_site(s->R(), 1, s->rot(), 1);
    dE = get_interaction_energy(&temp_site, neighbors, N1, N2) - get_chemical_potential(s, T);
    (*delta_bonds)[0] = N1; 
    (*delta_bonds)[1] = N2;
    return dE;
  }
}


//--------------------------------------------------
//Energy difference for rotating a center site
double Interaction::get_rot_energy_difference(Site* s, Lattice::NeighborVect neighbors, 
                                              int plus_minus, vector<int>* delta_bonds){
  double dE = 0;
  int N1 = 0;
  int N2 = 0;
  if (s->occ()){
    //call Site constructor with no random number generator for speed
    Site temp_site(s->R(), 1, s->rot()+plus_minus, 1, 0);
    dE += get_interaction_energy(&temp_site, neighbors, N1, N2);
    (*delta_bonds)[0] =  N1; 
    (*delta_bonds)[1] =  N2;

    dE -= get_interaction_energy(  s,        neighbors, N1, N2);
    (*delta_bonds)[0] -= N1; 
    (*delta_bonds)[1] -= N2;
    return dE;
  }
  return 0;
}


//--------------------------------------------------
//Chemical potential function for a single site
double Interaction::get_chemical_potential(Site* s, double T){
  if (m_lattice_being_tracked != 0){
    return (-m_J * m_lattice_being_tracked->z() / 2.0) 
      - T * log( m_lattice_being_tracked->R() );
  }
  else
    return 0;
}



int TestInteractionCode(){
  MTRand rng;
  vector<int> dimensions;
  dimensions.push_back(5);dimensions.push_back(10);
  SquareLattice my_lattice;
  Interaction my_interaction;
  my_lattice = SquareLattice(Lattice::LIQUID, dimensions, 4, &rng);
  my_interaction = Interaction(1, .33, .7, 2, 4, &my_lattice);
  

  vector<int> coord(2,0);
  coord[0] = 2 ; coord[1] = 7;
  
  //for some reason, the get_site(int) wraps around to the next row at 4.
  Site* s = my_lattice.get_site(coord);
  Lattice::NeighborVect* n = my_lattice.get_neighbors(coord);

  //Lattice function:
  my_lattice.Print();


  for (unsigned int i = 0; i < n->size() ; i++){
    cout << "Neighbor has occ: " << n->at(i)->occ() << " rot: "<<n->at(i)->rot()<<endl;
    
  }

  //interaction behavior
  int N1_test, N2_test;
  double my_interaction_energy = 
    my_interaction.get_interaction_energy(s, *n, N1_test,N2_test);
  
  cout <<"Interaction energy: " << my_interaction_energy <<endl;

  return 0;
}
