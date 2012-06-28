//monteCarlo.cpp

#include  "monteCarlo.h"

MonteCarlo::MonteCarlo(double J_in, double Q_in, double Q2_in,
                       int R_in, double T_in, double pdel_in)
  :m_delete_probability(pdel_in), m_T(T_in)
{
  m_rng = MTRand();
  vector<int> dimensions;
  dimensions.push_back(5);dimensions.push_back(10);
  m_lattice = SquareLattice(Lattice::SOLID, dimensions, R_in, &m_rng);
  m_interaction = Interaction(J_in, Q_in, Q2_in, 2, 4, &m_lattice);
}


void MonteCarlo::DoMetropolisMove(){
  Site* current_site;
  Lattice::NeighborVect* current_neighbors;
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
        (1.0/(m_delete_probability * current_site->R()))*
        exp(-m_interaction.get_occ_energy_difference
            (current_site, *current_neighbors, m_T, &delta_bonds)/m_T);
      //      cout << "Transition from occ probability: " << transition_probability << endl;
      current_site->AttemptOcc(transition_probability, m_rng);
    }
    else{
      int plus_minus;
      if (rand() < .5) plus_minus = 1 ; 
      else plus_minus = -1;
      transition_probability = 
        exp(-m_interaction.get_rot_energy_difference
            (current_site, *current_neighbors, plus_minus, &delta_bonds)/m_T);
      //      cout << "Rotation probability: " << transition_probability << endl;
      current_site->AttemptRot(plus_minus, transition_probability, m_rng);
    }
  }
  else{
    current_site->RandRot(m_rng);
    transition_probability = 
      (m_delete_probability * current_site->R())*
      exp(-m_interaction.get_occ_energy_difference
          (current_site, *current_neighbors, m_T, &delta_bonds)/m_T);    
    //    cout << "Transition from !occ Probability: " << transition_probability << endl;
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


void MonteCarlo::TestNeighborPointers(){
  vector<int> coord(2,0);
  coord[0] = 2 ; coord[1] = 2;

  Site* s = m_lattice.get_site(coord);
  cout << "2,2 has occ: "<<s->occ()<<" and rot: "<<s->rot()<<endl;
  cout << "Address: "<<s<<endl;

  coord[0] = 2 ; coord[1] = 3;
  s = m_lattice.get_site(coord);
  cout << "2,3 has occ: "<<s->occ()<<" and rot: "<<s->rot()<<endl;
  cout << "Address: "<<s<<endl;

  coord[0] = 2 ; coord[1] = 1;
  s = m_lattice.get_site(coord);
  cout << "2,1 has occ: "<<s->occ()<<" and rot: "<<s->rot()<<endl;
  cout << "Address: "<<s<<endl;

  coord[0] = 1 ; coord[1] = 2;
  s = m_lattice.get_site(coord);
  cout << "1,2 has occ: "<<s->occ()<<" and rot: "<<s->rot()<<endl;
  cout << "Address: "<<s<<endl;

  coord[0] = 3 ; coord[1] = 2;
  s = m_lattice.get_site(coord);
  cout << "3,2 has occ: "<<s->occ()<<" and rot: "<<s->rot()<<endl;
  cout << "Address: "<<s<<endl;
  
  coord[0] = 2 ; coord[1] = 2;
  Lattice::NeighborVect* n = m_lattice.get_neighbors(coord);
  for (unsigned int i = 0 ; i < n->size() ; i++){
    cout<<"Neighbor "<< i <<" has occ: "<<n->at(i)->occ()<<" and rot: "<<n->at(i)->rot()<<endl;
    cout << "Address: "<<n->at(i)<<endl;
  }


}



int TestMonteCarloCode(){
  MonteCarlo m;

  for (int i = 0 ; i < 10 ; i++){
    m.DoMetropolisSweep();
    m.PrintLattice();
  }

  return 0;
}
