//monteCarlo.cpp

#include "montecarlo.h"
#include "site.h"

MonteCarlo::MonteCarlo(double J_in, double Q_in, double Q2_in,
                       int R_in, double T_in, double pdel_in)
  :m_delete_probability(pdel_in), m_T(T_in)
{
  m_rng = MTRand();
  vector<int> dimensions;
  dimensions.push_back(5);dimensions.push_back(10);
  m_lattice = SquareLattice(Lattice::SOLID, dimensions, R_in, &m_rng);
  m_interaction = Interaction(J_in, Q_in, Q2_in, 2, 4, &m_lattice);
  m_file_handler = new MonteCarloFile(*this);
}


//Requires an include of "site.h"
void MonteCarlo::DoMetropolisMove(){
  Site* current_site;
  Lattice::Coord current_coord;
  Lattice::NeighborVect* current_neighbors;
  double transition_probability;
  vector<int> new_N1_bonds;
  vector<int> new_N2_bonds;
  bool moved;
  int old_rot;
  
  //points current_site to a random site, and write the coordinate to current_coord
  // and the neighbor-pointers to current_neighbors.
  current_site = m_lattice.random_site(m_rng, &current_neighbors, &current_coord);

  if (current_site->occ()){
    //The site is occupied
    old_rot = current_site->rot();

    if (m_rng.rand() < m_delete_probability){
      //We have selected a deletion move
      //
      //Note: Energy is always zero for an unoccupied site
      //      Thus, delta-E = 0 - get_interaction_energy
      transition_probability = 
        (1.0/(m_delete_probability * current_site->R()))*
        exp(-m_interaction.get_occ_energy_difference
            (current_site, *current_neighbors, m_T, new_N1_bonds, new_N2_bonds)/m_T);
      
      moved = current_site->AttemptOcc(transition_probability, m_rng);
    }

    else{
      //We have selected a rotation move
      int plus_minus;
      plus_minus = (rand() < .5)? 1:-1 ; 
      transition_probability = 
        exp(-m_interaction.get_rot_energy_difference
            (current_site, *current_neighbors, plus_minus, new_N1_bonds, new_N2_bonds)/m_T);

      moved = current_site->AttemptRot(plus_minus, transition_probability, m_rng);
    }
  }
  else{
    //The site is unoccupied and we have selected an insertion move in a random position
    old_rot = -1;

    current_site->RandRot(m_rng);
    transition_probability = 
      (m_delete_probability * current_site->R())*
      exp(-m_interaction.get_occ_energy_difference
          (current_site, *current_neighbors, m_T, new_N1_bonds, new_N2_bonds)/m_T);    

    moved = current_site->AttemptOcc(transition_probability, m_rng);
  }

  if (moved)
    m_interaction.UpdateOrderParameters(current_coord, old_rot, new_N1_bonds, new_N2_bonds);
}


void MonteCarlo::DoMetropolisSweep(){
  for (int i = 0 ; i < m_lattice.number_of_sites() ; i++){
    DoMetropolisMove();
  }
}


void MonteCarlo::PrintOrderParameters(){
  cout << "Rho:   "<<m_interaction.rho()   <<"\t"<<endl;
  cout << "Tau:   "<<m_interaction.get_N1()<<"\t"
       <<m_interaction.N1_symmetry_number()<<"\t"
       <<m_interaction.N1_division()<<endl;
  cout << "Omega: "<<m_interaction.get_N2()<<"\t"
       <<m_interaction.N2_symmetry_number()<<"\t"
       <<m_interaction.N2_division()<<endl;
}



int TestMonteCarloCode(){
  MonteCarlo m;

  for (int i = 0 ; i < 10 ; i++){
    m.DoMetropolisSweep();
    m.PrintLattice();
  }

  return 0;
}
