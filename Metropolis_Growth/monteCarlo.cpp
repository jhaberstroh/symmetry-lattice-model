//monteCarlo.cpp

#include  "monteCarlo.h"

MonteCarlo::MonteCarlo(double J_in, double Q_in, double Q2_in,
                       int R_in, double T_in, double pdel_in)
  :m_delete_probability(pdel_in), m_T(T_in)
{
  m_rng = MTRand();
  vector<int> dimensions;
  dimensions.push_back(5);dimensions.push_back(10);
  

  m_lattice = SquareLattice(Lattice::LIQUID, dimensions, R_in, &m_rng);

  m_interaction = Interaction(J_in, Q_in, Q2_in, 2, 4, &m_lattice);
}

void MonteCarlo::DoMetropolisMove(){
  Site* current_site;
  Lattice::NeighborVect current_neighbors;
  double transition_probability;
  //Interaction::OrderParameterType op;
  //delta_bonds[0] is for N1, delta_bonds[1] is for N2
  vector<int> delta_bonds(2,0);
  
  current_site = m_lattice.random_site(m_rng,&current_neighbors);
  if (current_site->occ()){
    if (m_rng.rand() < m_delete_probability){
      //Energy is always zero for an unoccupied site
      //Thus, delta-E = 0 - get_interaction_energy
      transition_probability = 
        (1/(m_delete_probability * current_site->R()))*
        exp(-m_interaction.get_occ_energy_difference(current_site, current_neighbors, m_T, &delta_bonds) 
            /m_T);
      current_site->AttemptOcc(transition_probability, m_rng);
    }
    else{
      int plus_minus;
      if (rand() < .5) plus_minus = 1 ; 
      else plus_minus = -1;
      transition_probability = 
        exp(-m_interaction.get_rot_energy_difference(current_site, current_neighbors, plus_minus, &delta_bonds) 
            /m_T);
      current_site->AttemptRot(plus_minus, transition_probability, m_rng);
    }
  }
  else{
    current_site->RandRot(m_rng);
    transition_probability = 
      (m_delete_probability * current_site->R())*
      exp(-m_interaction.get_occ_energy_difference(current_site, current_neighbors, m_T, &delta_bonds) 
           /m_T);    
    current_site->AttemptOcc(transition_probability, m_rng);
  }
  //      vector<int> opts;
  //      opts.push_back(dOcc); opts.push_back(rot); opts.push_back(plus_minus); opts.push_back(dN1bond);
  //TODO: Write to orderparmaeters after calculation!

}

void MonteCarlo::DoMetropolisSweep(){
  for (int i = 0 ; i < m_lattice.number_of_sites() ; i++){
    DoMetropolisMove();
  }
}

//void MonteCarlo::ResetOP(){
  //TODO: (jhaberstroh@lbl.gov) implement!
//}

void MonteCarlo::Track(){
  //TODO: (jhaberstroh@lbl.gov) implement!
}





int TestMonteCarloCode(){
  MonteCarlo m;

  for (int i = 0 ; i < 10 ; i++){
    m.DoMetropolisSweep();
    m.PrintLattice();
  }

  return 0;
}
