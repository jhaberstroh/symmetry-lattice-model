//Lattice.cpp



#include "Lattice.h"


const double Lattice::Jdft = 1;
const double Lattice::Qdft = .3;
const double Lattice::Q2dft = 1.7;
const double Lattice::Tdft = 1;
const double Lattice::pdeldft = .2;
const int    Lattice::Rdft = 32;
const vector<int> Lattice::sizedft (2,40);
const Lattice::Phase  Lattice::phasedft = LIQUID;
const Lattice::Interaction Lattice::itrdef = NEMATIC;



Lattice::Lattice(double J_in, double Q_in,     double Q2_in, int R_in,
                 double T_in, double pdel_in, MTRand* rng_in)
  : J(J_in), Q(Q_in), Q2(Q2_in), R(R_in), T(T_in), pdel(pdel_in)
{
  //initLat(t_in,dim_in,sizes_in,phase_in,R_in,rng_in); CANNOT CALL THIS! Call individually in each sub-constructor.
  if (rng_in == 0){
    new_rng = 1;
    rng = new MTRand();
  }
  else{
    new_rng = 0;
    rng = rng_in;
  }
}



Lattice::~Lattice(){
  for (int i = 0; i < n_sites ; i++){           \
    delete sites[i];
  }

  if (new_rng) delete rng;
}






void Lattice::metro_move(){
  int pulled = pull_random_site();
  Site::svec neighbors = pull_neighbors(pulled);

  //pdel is a factor which controls ratios of occupy moves to rotation moves. 
  
  if (sites[pulled]->get_occ()){
    if (rng->rand() < pdel)
      E += sites[pulled]->attempt_occ(neighbors, pdel);
    else
      E += sites[pulled]->attempt_rot(neighbors);
  }
  else
    E += sites[pulled]->attempt_occ(neighbors, pdel);
}




int Lattice::pull_random_site(){
  return rng->rand(n_sites-1); //A random number between [0,sites-1]
}





/* ------------------------------------------------------------------
   Square lattice functions
   ------------------------------------------------------------------ */


void SquareLattice::initLat(vector<int> sizes_in, Phase phase_in, Interaction itr_in){
  if (sizes_in.size() != 2){
    cout << "For a square lattice, we must be given two dimesions for input.\n";
    exit(2);
  }
  
  n_sites = sizes_in[0] * sizes_in[1];
  sizes = sizes_in;
  
  
  sites = Site::svec(n_sites);
  
  for (int i = 0 ; i < n_sites; i++){
    switch (phase_in){
    case GAS:
      //for a gas, the occupation probability will be zero.
      if (itr_in == NEMATIC){
        sites[i] = new NemSite(4, 0, rng, J, Q, Q2, R);
      }
      break;
    case LIQUID:
      //for a vapor, the occupation probability will be 1/2.
      if (itr_in == NEMATIC){
        sites[i] = new NemSite(4, .5, rng, J, Q, Q2, R);
      }
      break;
    default:
      cout << "Other initial phases are not yet defined.\n";
    }
  }
  
  cout << "In constructor SquareLattice::initLat; finding initial energy \n";
  cout << "Current size of m_sites array: "<<sites.size() <<endl;


  find_initial_energy();
}
 
 
double SquareLattice::find_initial_energy(){
  double energy = 0;
  vector<int> pos(2);
  Site* currentSite;
  for (int i = 0 ; i < sizes[0] ; i++){
    for (int j = 0 ; j < sizes[1] ; j++){
      //cout << "Current size of m_sites array: "<<sites.size() <<endl;

      pos[0]=i;   pos[1]=j;
      currentSite = get_site(pos);

      pos[0]=i+1; pos[1]=j;
      energy += currentSite->curr_interaction(get_site(pos));

      pos[0]=i;   pos[1]=j+1;
      energy += currentSite->curr_interaction(get_site(pos));
    }
  }
  return energy;
}

double SquareLattice::find_initial_phi(){
  double rho = 0;
  vector<int> pos(2);
  Site* currentSite;
  for (int i = 0 ; i < sizes[0] ; i++){
    for (int j = 0 ; j < sizes[1] ; j++){
      pos[0]=i;   pos[1]=j;
      currentSite = get_site(pos);

      rho += currentSite->get_occ();
    }
  }
  return ((rho/n_sites)*2)-1;
}

double SquareLattice::find_initial_tau(){
  double tau = 0;
  vector<int> pos(2);
  Site* currentSite;
  Site* otherSite;
  for (int i = 0 ; i < sizes[0] ; i++){
    for (int j = 0 ; j < sizes[1] ; j++){
      //cout << "Current size of m_sites array: "<<sites.size() <<endl;

      pos[0]=i;   pos[1]=j;
      currentSite = get_site(pos);

      pos[0]=i+1; pos[1]=j;
      otherSite = get_site(pos);
      tau += (currentSite->get_rot() ==  otherSite->get_rot() );
      tau -= (currentSite->get_rot() == (otherSite->get_rot()+R/2)%R);

      pos[0]=i;   pos[1]=j+1;
      otherSite = get_site(pos);
      tau += (currentSite->get_rot() ==  otherSite->get_rot() );
      tau -= (currentSite->get_rot() == (otherSite->get_rot()+R/2)%R);
    }
  }
  return tau/n_sites/2;
}

double SquareLattice::find_initial_omega(){
  double omega = 0;
  vector<int> pos(2);
  Site* currentSite;
  Site* otherSite;
  for (int i = 0 ; i < sizes[0] ; i++){
    for (int j = 0 ; j < sizes[1] ; j++){
      //cout << "Current size of m_sites array: "<<sites.size() <<endl;

      pos[0]=i;   pos[1]=j;
      currentSite = get_site(pos);

      pos[0]=i+1; pos[1]=j;
      otherSite = get_site(pos);
      omega += (currentSite->get_rot() ==  otherSite->get_rot() );
      omega += (currentSite->get_rot() == (otherSite->get_rot()+  R/2)%R);
      omega -= (currentSite->get_rot() == (otherSite->get_rot()+3*R/4)%R);
      omega -= (currentSite->get_rot() == (otherSite->get_rot()+  R/4)%R);

      pos[0]=i;   pos[1]=j+1;
      otherSite = get_site(pos);
      omega += (currentSite->get_rot() ==  otherSite->get_rot() );
      omega += (currentSite->get_rot() == (otherSite->get_rot()+  R/2)%R);
      omega -= (currentSite->get_rot() == (otherSite->get_rot()+3*R/4)%R);
      omega -= (currentSite->get_rot() == (otherSite->get_rot()+  R/4)%R);
    }
  }
  return omega/n_sites/2;
}




//This function returns a site, given by periodic boundary conditions
//First input is row, second is column.
Site* SquareLattice::get_site(vector<int> coords){
  if (coords.size() != 2){
    cout << "Number of coordinates given: " << coords.size() << endl;
    cout << "SquareLattice::get_site(int[]) must recieve two integers for coordinates!. Returning Null pointer.\n";
    return 0;
  }
  return sites[(((coords[0] % sizes[0]) + sizes[0]) % sizes[0] )* sizes[0] + 
               (((coords[1] % sizes[1]) + sizes[1]) % sizes[1])];

}


Site::svec SquareLattice::pull_neighbors(int site){
  if (site >= n_sites){
    cout << "In SquareLattice::pull_neighbors(int), the input number was larger than the array size.\n\tIt was shifted modulo n_sites down for covenience.\n ";
    site = site % n_sites;
  }
  if (site < 0){
    cout << "In SquareLattice::pull_neighbors(int), the input number was negative.\n\tIt was shifted modulo n_sites up for covenience.\n";
    site = ((site % n_sites)+n_sites)%n_sites;
  }

  if (site < n_sites && site >= 0){
    
    int row = site/sizes[0];
    int col = site%sizes[0];
    
    int n1 = ((((row - 1) % sizes[0]) + sizes[0]) % sizes[0]) * sizes[0] + col;           //shift row back
    int n2 = ((((row + 1) % sizes[0]) + sizes[0]) % sizes[0]) * sizes[0] + col;           //shift row forward
    int n3 =  (((col - 1) % sizes[1]) + sizes[1]) % sizes[1]             + row * sizes[0];  //shift col back
    int n4 =  (((col + 1) % sizes[1]) + sizes[1]) % sizes[1]             + row * sizes[0];  //shift col forward
    
    Site::svec nghbors(4);
    
    nghbors[0] = sites[n1];
    nghbors[1] = sites[n2];
    nghbors[2] = sites[n3];
    nghbors[3] = sites[n4];

    return nghbors;
  }

  else{
    cout << "Major error in SquareLattice::pull_neighbors";
    exit(3);
  }
  

}


void SquareLattice::printLat(){

  /*
  prints like the following:

/-------------------\
|  2  3 12 11  .  3 |
|                   |
|  2  3 12  . 20  3 |
|                   |
|  2  . 12  . 20  3 |
\___________________/


  */

  vector<int> pos(2);
  Site* posSite;
  int posRot;

  for (int i = 0 ; i < sizes[0] ; i++){

    //prints the first line for the sake of nice formatting
    if (i == 0){
      cout << "/";
      for (int j = 0 ; j < sizes[1] ; j++){     
        cout << "---";
      }
      cout << "-\\" << endl;
    }
    //prints the blank lines above each line when not the first line.
    else{
      cout << "|";
      for (int j = 0 ; j < sizes[1] ; j++){     
        cout << "   ";
      }
      cout << " |" <<endl;
    }


    cout << "|";
    for (int j = 0 ; j < sizes[1] ; j++){
      pos[0] = i;
      pos[1] = j;

      posSite = get_site(pos);
      
      if (posSite->get_occ()){

        posRot = posSite->get_rot();
        if (posRot < 10)
          cout << "  " << posRot;
        else if (posRot < 100)
          cout << " " << posRot;
        else
          cout << "PRINT ERROR: values too large.\n";

      }
      else{
        cout << "  .";
        
      }
    }
    cout << " |" << endl;



    //prints the last line for the sake of nice formatting
    if (i == sizes[0] - 1){
      cout << "\\";
      for (int j = 0 ; j < sizes[1] ; j++){     
        cout << "___";
      }
      cout << "_/" << endl;
    }
    
  }
}
