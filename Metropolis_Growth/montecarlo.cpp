//monteCarlo.cpp

#include  "montecarlo.h"

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
  //      vector<int> opts;
  //      opts.push_back(dOcc); opts.push_back(rot); opts.push_back(plus_minus); opts.push_back(dN1bond);
  //TODO: Write to orderparmaeters after calculation!
  
  


}


void MonteCarlo::DoMetropolisSweep(){
  for (int i = 0 ; i < m_lattice.number_of_sites() ; i++){
    DoMetropolisMove();
  }
}

void MonteCarlo::FileHeader(){
  m_file_buffer.str("");

  m_file_buffer << "# R = "<<m_lattice.R()<<"\n";
  m_file_buffer << "# J = "<<m_interaction.J()<<"\n";
  m_file_buffer << "# Q"<<m_interaction.N1_symmetry_number()<<" = "<<m_interaction.QN1()<<"\n";
  m_file_buffer << "# Q"<<m_interaction.N2_symmetry_number()<<" = "<<m_interaction.QN2()<<"\n";
  m_file_buffer << "# T = "<<m_T<<"\n";
  m_file_buffer << "# pdel = "<<m_delete_probability<<"\n";
  m_file_buffer << "# rho\trho_sq\tN1_tau\tN1_tau_sq\tN2_tau\tN2_tau_sq\n";
  
  if (m_output_file.is_open())
    m_output_file << m_file_buffer.str();
  else
    cout << "Minor error: File must be open to write to the header\n\n"
         << "Action skipped." <<endl;

  m_file_buffer.str("");
}

void MonteCarlo::SetupTrack(vector<FNameOpt> fname_include, ios_base::openmode open){
  m_file_name.str("");
  m_file_name << "ordPar_R"<<m_lattice.R();
  for (unsigned int i = 0 ; i < fname_include.size() ; i++){
    switch (fname_include[i]){
    case kJ:
      m_file_name << "_J-"<<m_interaction.J();
      break;
    case kQN1:
      m_file_name << "_Q"<<m_interaction.N1_symmetry_number()<<"-"<<m_interaction.QN1();
      break;
    case kQN2:
      m_file_name << "_Q"<<m_interaction.N2_symmetry_number()<<"-"<<m_interaction.QN2();
      break;      
    case kT:
      m_file_name << "_T-"<<m_T;
      break;      
    case kPDel:
      m_file_name << "_PD-"<<m_delete_probability;
      break;
    default:
      m_file_name << "_na_";
      break;
    }
  }
  //Comma-separated value list
  m_file_name << ".csv"; 

  while (m_output_file.is_open()){
    m_output_file.flush();
    m_output_file.close();
  }
  if (!m_output_file.is_open()){
    //open the file with the selected ios_base::openmode
    //Some choices of interest follow:
    //  ios_base::app -> all writes will go to the end of the file
    //  ios_base::ate -> opens initially at end of the file
    //  ios_base::trunc -> deletes everything in the file and opens at the beginning
    const char* c_file_name = m_file_name.str().c_str();
    m_output_file.open(c_file_name, open);
    FileHeader();
    m_output_file.close();
    m_openmode = open;
  }
}




void MonteCarlo::SetupTrack(){
  vector<FNameOpt> fname_include(3);
  fname_include[0] = kJ;
  fname_include[1] = kQN1;
  fname_include[2] = kQN2;

  SetupTrack(fname_include);
}




void MonteCarlo::Track(){
  cout <<"Tracking!" <<endl;
  m_file_buffer.str("");
  
  m_file_buffer << m_interaction.rho() << "\t"
                << m_interaction.rho() * m_interaction.rho()<< "\t"
                << m_interaction.get_N1() / 2<< "\t"
                <<(m_interaction.get_N1() / 2)*(m_interaction.get_N1() / 2)<< "\t"
                << m_interaction.get_N2() / 2<< "\t"
                <<(m_interaction.get_N2() / 2)*(m_interaction.get_N2() / 2)<<endl;


  const char* c_file_name = m_file_name.str().c_str();
  m_output_file.open(c_file_name, m_openmode);
  m_output_file << m_file_buffer.str();
  m_output_file.close();
  m_file_buffer.str("");

}



















/*--------------------------------------------------
  Debugging Code
  --------------------------------------------------*/





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
