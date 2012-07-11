//monteCarlo.cpp

#include "montecarlo.h"
#include "site.h"


MonteCarlo::MonteCarlo(double J_in, double Q_in, double Q2_in,
                       int R_in, double T_in, double pdel_in, vector<int> measurements)
  :m_delete_probability(pdel_in), m_T(T_in), m_rng(MTRand()), 
{
  m_lattice = SquareLattice(Lattice::SOLID, measurements, R_in, &m_rng);
  m_interaction = Interaction(J_in, Q_in, Q2_in, 2, 4, &m_lattice);
  m_file_handler = MonteCarloFile(this);
}


//Requires an include of "site.h"
void MonteCarlo::DoMetropolisMove(){
  Site* current_site;
  Lattice::NeighborVect* current_neighbors;
  double transition_probability;
  //Interaction::OrderParameterType op;
  //delta_bonds[0] is for N1, delta_bonds[1] is for N2
  vector<int> delta_bonds(2,0);
  //modifiers for the order parameter (for Interaction::update_order_parameters)
  vector<int> mod(4,0);
  
  current_site = m_lattice.random_site(m_rng,&current_neighbors);
  if (current_site->occ()){
    if (m_rng.rand() < m_delete_probability){
      //Energy is always zero for an unoccupied site
      //Thus, delta-E = 0 - get_interaction_energy
      transition_probability = 
        (1.0/(m_delete_probability * current_site->R()))*
        exp(-m_interaction.get_occ_energy_difference
            (current_site, *current_neighbors, m_T, &delta_bonds)/m_T);
      //      cout << "Transition from occ probability: " << transition_probability << endl;
      if (current_site->AttemptOcc(transition_probability, m_rng)){
        mod = vector<int>(1,0); 
        mod[0] = -1;
        m_interaction.update_order_parameters(Interaction::kOrderTypeOcc, mod);
        mod = vector<int>(4,0); 
        mod[0] = -1; mod[1] = current_site->R();
        mod[2] =  0; mod[3] = delta_bonds[0];
        cout << "delta_bonds from deletion: " <<mod[3]<<endl;
        m_interaction.update_order_parameters(Interaction::kOrderTypeN1, mod);
        mod[0] = -1; mod[1] = current_site->R();
        mod[2] =  0; mod[3] = delta_bonds[1];
        m_interaction.update_order_parameters(Interaction::kOrderTypeN2, mod);
      }
    }
    else{
      int plus_minus;
      if (rand() < .5) plus_minus = 1 ; 
      else plus_minus = -1;
      transition_probability = 
        exp(-m_interaction.get_rot_energy_difference
            (current_site, *current_neighbors, plus_minus, &delta_bonds)/m_T);
      //      cout << "Rotation probability: " << transition_probability << endl;
      if (current_site->AttemptRot(plus_minus, transition_probability, m_rng)){
        mod = vector<int>(4,0); 
        mod[0] = 0; mod[1] = current_site->R();
        mod[2] = plus_minus; mod[3] = delta_bonds[0];
        m_interaction.update_order_parameters(Interaction::kOrderTypeN1, mod);
        mod[0] = 0; mod[1] = current_site->R();
        mod[2] = plus_minus; mod[3] = delta_bonds[1];
        m_interaction.update_order_parameters(Interaction::kOrderTypeN2, mod);

      }
    }
  }
  else{
    current_site->RandRot(m_rng);
    transition_probability = 
      (m_delete_probability * current_site->R())*
      exp(-m_interaction.get_occ_energy_difference
          (current_site, *current_neighbors, m_T, &delta_bonds)/m_T);    
    //    cout << "Transition from !occ Probability: " << transition_probability << endl;
    if (current_site->AttemptOcc(transition_probability, m_rng)){
      mod = vector<int>(1,0); 
      mod[0] = 1;
      m_interaction.update_order_parameters(Interaction::kOrderTypeOcc, mod);
      mod = vector<int>(4,0);
      mod[0] = 1; mod[1] = current_site->R();
      mod[2] = 0; mod[3] = delta_bonds[0];
      m_interaction.update_order_parameters(Interaction::kOrderTypeN1, mod);
      mod[0] = 1; mod[1] = current_site->R();
      mod[2] = 0; mod[3] = delta_bonds[1];
      m_interaction.update_order_parameters(Interaction::kOrderTypeN2, mod);    
    }
  }
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
