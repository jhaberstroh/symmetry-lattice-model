//interaction.cpp
#include "interaction.h"

/*--------------------------------------------------
  Order Parameter functions
  --------------------------------------------------*/


int Interaction::InitRho(){
  Lattice* l = m_lattice_being_tracked;
  m_rho = 0;
  vector<int> coord(2);
  for (int i = 0 ; i < l->measurements()[0] ; i++){
    for (int j = 0 ; j < l->measurements()[1] ; j++){
      coord[0]=i;   coord[1]=j;
      m_rho += (l->view_site(coord)[0] != -1);
    }
  }
  return m_rho;
}


int Interaction::InitNAligned(int symmetry_num, vector<int>* n_aligned_direction){
  Lattice* l = m_lattice_being_tracked;
  //reset the array which tracks how many sites point in a given "generalized" direction
  // (directions which contribute to one another due to symmetry count as the same direction)
  n_aligned_direction->clear();
  n_aligned_direction->resize(l->R()/symmetry_num, 0);
  vector<int> coord(2);
  vector<int> current_site;
  for (int i = 0 ; i < l->measurements()[0] ; i++){
    for (int j = 0 ; j < l->measurements()[1] ; j++){
      coord[0]=i;   coord[1]=j;
      current_site = l->view_site(coord);

      if (current_site[0] != -1){
        if (current_site[0] == l->R()){
          //LHS: Uses only the range of [0, R/symmetry_num-1]
          //RHS: Maps alternating sections to +1 and -1
          (*n_aligned_direction)[current_site[0] % (l->R()/2)] 
            += (((current_site[0] / (l->R()/symmetry_num)) % symmetry_num) * (-2)) + 1;
        }
        //TODO:throw container_value_mismatch_error() if R's in different places are not equal.
      }
    }
  }
  return abs(*max_element(n_aligned_direction->begin(), n_aligned_direction->end(), abs_compare));
}


int Interaction::InitNBond(int symmetry_num, vector<int>* n_bond_lattice){
  Lattice* l = m_lattice_being_tracked;
  (*n_bond_lattice) = l->CreateBondVector();
  Lattice::Coord coord(2);

  //rot-values for current site, to fill in from get_site.
  vector<int> current_site;
  int division_size = l->R() / symmetry_num;
  if (l->R() % symmetry_num != 0){
    throw bad_symmetry_number("Interaction::ComputeNBond", l->R(), symmetry_num);
  }
  for (int i = 0 ; i < l->measurements()[0] ; i++){
    for (int j = 0 ; j < l->measurements()[1] ; j++){
      coord[0] = i; coord[1] = j;
      current_site = l->view_site(coord);

      if (current_site[0] != -1){
        for (unsigned int k = 1 ; k < current_site.size() ; k++){
          if (current_site[k] != -1){

            //If their alignments are such that they will have bonding or repulsion...
            if ((current_site[k] % division_size) == (current_site[0] % division_size)){
              
              //Second argument of LookupBondIndex is the direction; this is synchronized
              // with the choices within subclasses of SquareLattice because those classes
              // build the neighbor arrays!
              (*n_bond_lattice)[l->LookupBondIndex(coord, k-1)] +=
                //If the sites are bonding, we get a contribution of +1
                //If the sites are repelling, we get a contribution of -1
                (((current_site[k]/division_size)%2) == ((current_site[0]/division_size)%2))? 1:-1;
            }
          }
        }
      }
    }
  }
  for (unsigned int i = 0 ; i < n_bond_lattice->size() ; i++){
    //Bonds will have been double counted; undo half of the counts!
    //But check for proper code function first...
    if ((*n_bond_lattice)[i] % 2 != 0)
      cout << "Big problem in Interaction::InitNBond; bonds are not double-counting!\n"
           << "n_bond_lattice["<<i<<"] = "<<(*n_bond_lattice)[i] <<endl;
    if ((*n_bond_lattice)[i] / 2 != 0 && (*n_bond_lattice)[i] / 2 != 1)
      cout << "Big problem in Interaction::InitNBond; bonds are counting poorly!\n"
           << "n_bond_lattice["<<i<<"] = "<<(*n_bond_lattice)[i] <<endl;

    (*n_bond_lattice)[i] /= 2;
    m_order_n1 += (*n_bond_lattice)[i];
  }
  return m_order_n1;
}


int Interaction::InitSpecificOP(OrderParameterType opt){
  switch (opt){
  case kOrderTypeOcc:
    InitRho();
    break;
  case kOrderTypeN1:
    if (kOrderN1Bond == false) throw not_implemented_error("Site-based directional order parameters");
    if (kOrderN1Bond == true)  InitNBond(m_N1, &m_N1_bond_lattice);
    break;
  case kOrderTypeN2:
    if (kOrderN2Bond == false) throw not_implemented_error("Site-based directional order parameters");
    if (kOrderN2Bond == true)  InitNBond(m_N2, &m_N2_bond_lattice);
    break;
  }
  return 0; ///UHHHH what's this here for? TODO for suuuure.
}
  
  
void Interaction::InitOrderParameters(){
  if (m_lattice_being_tracked != 0){
    Lattice* l = m_lattice_being_tracked;
    m_N1_bond_lattice = l->CreateBondVector();
    m_N2_bond_lattice = l->CreateBondVector();
    m_N1_division = l->R() / m_N1;
    m_N2_division = l->R() / m_N2;
    m_rho = 0;
    m_order_n1 = 0;
    m_order_n2 = 0;

    //Must initialize m_N*_bond_lattice
    InitSpecificOP(kOrderTypeOcc);
    InitSpecificOP(kOrderTypeN1);
    InitSpecificOP(kOrderTypeN2);
  }
}
 
 

void Interaction::UpdateOrderParameters(Lattice::Coord& coord, int old_rot, vector<int>& new_N1_bonds, vector<int>& new_N2_bonds){
  //new_r is -1 if not occupied, and a natural number (< R) if occupied.
  Lattice* l = m_lattice_being_tracked;
  Site* new_site = l->get_site(coord);
  int bond_index;

  if (old_rot >= 0 && new_site->occ()){
    //Both the old and new are occupied.
    if (old_rot == new_site->rot()){
      //No change was made.
    }
    else{
      //Only the rotational state changed.
      for (unsigned int i = 0 ; i < new_N1_bonds.size() ; i++){
        bond_index = l->LookupBondIndex(coord, i);

        m_order_n1 -= m_N1_bond_lattice[bond_index];
        m_N1_bond_lattice[bond_index] = new_N1_bonds[i];
        m_order_n1 += new_N1_bonds[i];

        m_order_n2 -= m_N2_bond_lattice[bond_index];
        m_N2_bond_lattice[bond_index] = new_N2_bonds[i];
        m_order_n2 += new_N2_bonds[i];
      }
    }
  }
  else{
    //There was a change in occupation level.
    if (old_rot == -1){
      //The old site was unoccupied (all bonds were 0).
      m_rho += 1;

      for (unsigned int i = 0 ; i < new_N1_bonds.size() ; i++){
        bond_index = l->LookupBondIndex(coord, i);

        m_N1_bond_lattice[bond_index] = new_N1_bonds[i];
        m_order_n1 += new_N1_bonds[i];

        m_N2_bond_lattice[bond_index] = new_N2_bonds[i];
        m_order_n2 += new_N2_bonds[i];
      }
    }
    else{
      //The new site is unoccupied (all bonds go to 0).
      m_rho -= 1;

      for (unsigned int i = 0 ; i < new_N1_bonds.size() ; i++){
        bond_index = l->LookupBondIndex(coord, i);

        m_order_n1 -= m_N1_bond_lattice[bond_index];
        m_N1_bond_lattice[bond_index] = 0;

        m_order_n2 -= m_N2_bond_lattice[bond_index];
        m_N2_bond_lattice[bond_index] = 0;
      }
    }
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
  retn_N1_bond = 0;
  retn_N2_bond = 0;

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
double Interaction::get_interaction_energy(Site* s, Lattice::NeighborVect& neighbors, 
                                           LocalBondVect& new_N1_bond, LocalBondVect& new_N2_bond){
  //TODO: (jhaberstroh@lbl.gov) optimize for redundant accesses on s.
  double energy = 0;
  int retn_N1_bond, retn_N2_bond;

  for (int i = 0; i < m_lattice_being_tracked->z(); i++){
    energy += get_interaction_energy(s, neighbors[i], retn_N1_bond, retn_N2_bond);
    new_N1_bond[i] += retn_N1_bond;
    new_N2_bond[i] += retn_N2_bond;
  }

  return energy;
}


//--------------------------------------------------
//Energy difference for flipping the occupation of a center site
double Interaction::get_occ_energy_difference(Site* s, Lattice::NeighborVect& neighbors, 
                                              double T, LocalBondVect& new_N1_bond, LocalBondVect& new_N2_bond){
  Lattice* l = m_lattice_being_tracked;
  double dE = 0;
  new_N1_bond.clear();
  new_N1_bond.resize(l->z(), 0);
  new_N2_bond.clear();
  new_N2_bond.resize(l->z(), 0);
  //Energy of unoccupied is always 0, we have easier computations!
  if (s->occ()){
    // (dE_insert = (dU_insert = 0 - U) - mu)
    dE = -get_interaction_energy(s, neighbors, new_N1_bond, new_N2_bond) + get_chemical_potential(s, T);
    new_N1_bond.clear();
    new_N1_bond.resize(l->z(), 0);
    new_N2_bond.clear();
    new_N2_bond.resize(l->z(), 0);
    return dE;
  }
  else{
    // (dE_insert = (dU_insert = U - 0) - mu)
    //Create a dummy occupied state
    Site temp_site(s->R(), 1, s->rot(), 1);
    dE = get_interaction_energy(&temp_site, neighbors, new_N1_bond, new_N2_bond) - get_chemical_potential(s, T);
    return dE;
  }
}


//--------------------------------------------------
//Energy difference for rotating a center site
double Interaction::get_rot_energy_difference(Site* s, Lattice::NeighborVect& neighbors, 
                                              int plus_minus, LocalBondVect& new_N1_bond, LocalBondVect& new_N2_bond){
  Lattice* l = m_lattice_being_tracked;
  double dE = 0;
  new_N1_bond.clear();
  new_N1_bond.resize(l->z(), 0);
  new_N2_bond.clear();
  new_N2_bond.resize(l->z(), 0);
  if (s->occ()){
    //old site:
    dE -= get_interaction_energy(  s,        neighbors, new_N1_bond, new_N2_bond);
    new_N1_bond.clear();
    new_N1_bond.resize(l->z(), 0);
    new_N2_bond.clear();
    new_N2_bond.resize(l->z(), 0);

    //new site:
    //call Site constructor with no random number generator for speed
    Site temp_site(s->R(), 1, s->rot()+plus_minus, 1, 0);
    dE += get_interaction_energy(&temp_site, neighbors, new_N1_bond, new_N2_bond);
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
  vector<int> N1_test, N2_test;
  double my_interaction_energy = 
    my_interaction.get_interaction_energy(s, *n, N1_test,N2_test);
  
  cout <<"Interaction energy: " << my_interaction_energy <<endl;

  return 0;
}
