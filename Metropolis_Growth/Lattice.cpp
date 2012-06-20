//Lattice.cpp



#include "Lattice.h"


const double Lattice::Jdft = 1;
const double Lattice::Qdft = .3;
const double Lattice::Q2dft = 1.7;
const double Lattice::Tdft = 1;
const double Lattice::pdeldft = .2;
const int    Lattice::Rdft = 32;
const vector<int> Lattice::sizedft (2,40);
const Lattice::Phase  Lattice::phasedft = SOLID;
const Lattice::Interaction Lattice::itrdef = NEMATIC;



Lattice::Lattice(double J_in, double Q_in,     double Q2_in, int R_in,
                 double T_in, double pdel_in, MTRand* rng_in)
  : R(R_in), T(T_in), pdel(pdel_in)
{
  //Should include some sort of check to assess whether R is a multiple of four...


  //initLat(t_in,dim_in,sizes_in,phase_in,R_in,rng_in); CANNOT CALL THIS! Call individually in each sub-constructor.
  params = Site::pvec(3); //constructor for a vector<double> to hold parameters
  params[0] = J_in;
  params[1] = Q_in;
  params[2] =Q2_in;

  if (rng_in == 0){
    new_rng = 1;
    rng = new MTRand();
  }
  else{
    new_rng = 0;
    rng = rng_in;
  }

  //These are INITIALIZED in findInitialTau and findInitialOmega
  //They are later UPDATED in metroMove and optMetroMove (NOT YET FINISHED June 12)
  directions = Site::dirtable();
  vector<int> tauDir(R/2,0);
  vector<int> omgDir(R/4,0);
  directions.push_back(tauDir);
  directions.push_back(omgDir);


  //filename = ostringstream();
  filename.setf(ios::fixed, ios::floatfield);
  filename.precision(4);
  filename.str("");
  filename << "metro_simulation_J_"<<params[0]<<"_Q_"<<params[1]<<"_Q2_"<<params[2]<<".txt";

}


Lattice::~Lattice(){
  for (int i = 0; i < n_sites ; i++){           \
    delete sites[i];
  }

  if (new_rng) delete rng;
}


void Lattice::metroMove(){
  int pulled = pullRandomSite();
  Site::svec neighbors = pullNeighbors(pulled);

  //pdel is a factor which controls ratios of occupy moves to rotation moves. 
  
  if (sites[pulled]->getOcc()){
    if (rng->rand() < pdel)
      E += sites[pulled]->attemptOcc(neighbors, pdel, T, params, &orders, &directions);
    else
      E += sites[pulled]->attemptRot(neighbors, T, params, &orders, &directions);
  }
  else
    E += sites[pulled]->attemptOcc(neighbors, pdel, T, params, &orders, &directions);
}

void Lattice::optimize(){
  for (int i = 0 ; i < n_sites ; i++)
    sites[i]->setNeigh(pullNeighbors(i));
}

void Lattice::optMetroMove(){
  int pulled = pullRandomSite();
    
  if (sites[pulled]->getOcc()){
    if (rng->rand() < pdel) //pdel controls the percentage of moves which go to rotation
      E += sites[pulled]->attemptOcc(pdel, T, params, &orders, &directions);
    else
      E += sites[pulled]->attemptRot(T, params, &orders, &directions);
  }
  else
    E += sites[pulled]->attemptOcc(pdel, T, params, &orders, &directions);
}



void Lattice::track(){
  //Note: Can be modified in the future to account for different
  //      order parameters.

  pFile = fopen(filename.str().c_str(),"a");
  
  fprintf(pFile, "%5.4f \t %1.4f \t %1.4f \t %1.4f \n", getE(),getRho(),getTau(),getOmega());
  fclose(pFile);
}



void Lattice::fileSetup(){

};



int Lattice::pullRandomSite(){
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
        sites[i] = new NemSite(4, 0, rng, R);
      }
      break;
    case LIQUID:
      //for a vapor, the occupation probability will be 1/2.
      if (itr_in == NEMATIC){
        sites[i] = new NemSite(4, .5, rng, R);
      }
      break;
    case SOLID:
      //for a solid, the occupation probability will be 1.
      if (itr_in == NEMATIC){
        sites[i] = new NemSite(4, 1, rng, R);
      }
      break;
    default:
      cout << "Other initial phases are not yet defined.\n";
    }
  }
  
  cout << "In constructor SquareLattice::initLat; finding initial energy \n";
  cout << "Current size of m_sites array: "<<sites.size() <<endl;


  E = findInitialEnergy();
  orders = Site::ovec(3);
  orders[0] = findInitialRho();
  orders[1] = findInitialTau();
  orders[2] = findInitialOmega();
}
 
 
double SquareLattice::findInitialEnergy(){
  double energy = 0;
  vector<int> pos(2);
  Site* currentSite;
  for (int i = 0 ; i < sizes[0] ; i++){
    for (int j = 0 ; j < sizes[1] ; j++){
      //cout << "Current size of m_sites array: "<<sites.size() <<endl;

      pos[0]=i;   pos[1]=j;
      currentSite = getSite(pos);

      pos[0]=i+1; pos[1]=j;
      energy += currentSite->currInteraction(getSite(pos), params);

      pos[0]=i;   pos[1]=j+1;
      energy += currentSite->currInteraction(getSite(pos), params);

      if (currentSite -> getOcc())
        energy -= currentSite->chemPotential(T, params);
    }
  }
  return energy;
}

double SquareLattice::findInitialRho(){
  double rho = 0;
  vector<int> pos(2);
  Site* currentSite;
  for (int i = 0 ; i < sizes[0] ; i++){
    for (int j = 0 ; j < sizes[1] ; j++){
      pos[0]=i;   pos[1]=j;
      currentSite = getSite(pos);

      rho += currentSite->getOcc();
    }
  }
  return rho;
}

double SquareLattice::findInitialTau(){
  vector<int> pos(2);
  Site* currentSite;
  
  for (int i = 0 ; i < directions.size() ; i++){
    cout << "Size of "<< i << "th element: " << directions[i].size() << endl;
    for (int j = 0 ; j < directions[i].size(); j++){
      directions[i][j] = 0;
    }
  }


  for (int i = 0 ; i < sizes[0] ; i++){
    for (int j = 0 ; j < sizes[1] ; j++){
      //cout << "Current size of m_sites array: "<<sites.size() <<endl;

      pos[0]=i;   pos[1]=j;
      currentSite = getSite(pos);
      
      
      //maps rotation in the first half to +1, in the second half to -1
      if (currentSite->getOcc()){
        //tauDir  is directions[0]
        //The index finds the slot for that direction
        //The adding part makes sure that "opposite" directions subtract
        directions[0][currentSite->getRot() % (R/2)] += (((currentSite->getRot() / (R/2)) % 2) * (-2)) + 1;
      }
    }
  }
  
  //Selects the direction with the maximum number of non-cancelling entries (largest direction)
  //Note: The order parameter is tau = <occ_i delta_ferro(rot_i, **some directions)>
  cout << "Max element is: "<< abs(*max_element(directions[0].begin(), 
                                                directions[0].end(),
                                                abs_compare)) << endl;
  return abs(*max_element(directions[0].begin(), 
                          directions[0].end(),
                          abs_compare));
}

double SquareLattice::findInitialOmega(){
  vector<int> pos(2);
  Site* currentSite;
  for (int i = 0 ; i < sizes[0] ; i++){
    for (int j = 0 ; j < sizes[1] ; j++){
      //cout << "Current size of m_sites array: "<<sites.size() <<endl;

      pos[0]=i;   pos[1]=j;
      currentSite = getSite(pos);
      
      if (currentSite->getOcc()){
        //omgDir  is directions[1]
        directions[1][currentSite->getRot() % (R/4)] += (((currentSite->getRot() / (R/4)) % 2) * (-2)) + 1;
      }
    }
  }
  return abs(*max_element(directions[1].begin(), 
                          directions[1].end(),
                          abs_compare));

}






//This function returns a site, given by periodic boundary conditions
//First input is row, second is column.
Site* SquareLattice::getSite(vector<int> coords){
  if (coords.size() != 2){
    cout << "Number of coordinates given: " << coords.size() << endl;
    cout << "SquareLattice::getSite(int[]) must recieve two integers for coordinates!. Returning Null pointer.\n";
    return 0;
  }
  return sites[(((coords[0] % sizes[0]) + sizes[0]) % sizes[0] )* sizes[0] + 
               (((coords[1] % sizes[1]) + sizes[1]) % sizes[1])];
  
}


Site::svec SquareLattice::pullNeighbors(int site){
  if (site >= n_sites){
    cout << "In SquareLattice::pullNeighbors(int), the input number was larger than the array size.\n\tIt was shifted modulo n_sites down for covenience.\n ";
    site = site % n_sites;
  }
  if (site < 0){
    cout << "In SquareLattice::pullNeighbors(int), the input number was negative.\n\tIt was shifted modulo n_sites up for covenience.\n";
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
    cout << "Major error in SquareLattice::pullNeighbors";
    exit(3);
  }
  

}


int SquareLattice::coordToIndex(vector<int> coord){
  return (  (((coord[0] % sizes[0]) + sizes[0]) % sizes[0])*sizes[0] 
          + (((coord[1] % sizes[1]) + sizes[1]) % sizes[1])           );

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

      posSite = getSite(pos);
      
      if (posSite->getOcc()){

        posRot = posSite->getRot();
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
