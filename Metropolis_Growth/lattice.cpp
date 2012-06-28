//Lattice.cpp
#include "lattice.h"

/*--------------------------------------------------
  Indexing functions & utilities
  --------------------------------------------------*/

int FindIndexOf(vector<Site*> array, Site* s){
  for (unsigned int i = 0 ; i < array.size() ; i++){
    if (s == array[i])
      return i;
  }
  return -1; //sentinel return value
}



//Result will treat input coordinates completely periodically;
// thus, it will not be the highest performance if input value is bounded,
// but it is extremely robust (great for readability).
int SquareLattice::CoordToIndex(vector<int> & coord){
  if (coord.size() == 2){
    //cout << "Coord to index: " << coord[0] <<","<<coord[1]<<" = "
    // << (((coord[0] %m_measurements[0]) +m_measurements[0]) %m_measurements[0]) * m_measurements[1]
    //  + (((coord[1] %m_measurements[1]) +m_measurements[1]) %m_measurements[1]) << endl;

      
    return (((coord[0] %m_measurements[0]) +m_measurements[0]) %m_measurements[0]) * m_measurements[1]
      + (((coord[1] %m_measurements[1]) +m_measurements[1]) %m_measurements[1]);
  }
  else{
    throw vector_size_error("SquareLattice::CoordToIndex:coord", 2, coord.size());
  }
}

vector<int> SquareLattice::IndexToCoord(int index){
  index = ((index % m_lattice.size()) +m_lattice.size()) %m_lattice.size();
  vector<int> coord_out(2,0);

  coord_out[0] = index / m_measurements[1];
  coord_out[1] = index % m_measurements[1];

  return coord_out;
}


void SquareLattice::Print(){
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
  vector<int> coord(2);
  Site* current_site;
  int current_rot;
  for (int i = 0 ; i < m_measurements[0] ; i++){
    //prints the first line for the sake of nice formatting
    if (i == 0){
      cout << "/";
      for (int j = 0 ; j < m_measurements[1] ; j++){     
        cout << "---";
      }
      cout << "-\\" << endl;
    }
    //prints the blank lines above each line when not the first line.
    else{
      cout << "|";
      for (int j = 0 ; j < m_measurements[1] ; j++){     
        cout << "   ";
      }
      cout << " |" <<endl;
    }
    cout << "|";
    for (int j = 0 ; j < m_measurements[1] ; j++){
      coord[0] = i;  coord[1] = j;
      current_site = get_site(coord);
      if (current_site->occ()){
        current_rot = current_site->rot();
        if (current_rot < 10)
          cout << "  " << current_rot;
        else if (current_rot < 100)
          cout << " " << current_rot;
        else
          cout << "PRINT ERROR: values too large.\n";
      }
      else{
        cout << "  .";
      }
    }
    cout << " |" << endl;
    //prints the last line for the sake of nice formatting
    if (i == m_measurements[0] - 1){
      cout << "\\";
      for (int j = 0 ; j < m_measurements[1] ; j++){     
        cout << "___";
      }
      cout << "_/" << endl;
    }
  }
}

/*--------------------------------------------------
  Constructors, Destructors, Assignment (Non-trivial!)
  --------------------------------------------------*/

Lattice::~Lattice(){
  for (unsigned int i = 0 ; i < m_lattice.size() ; i++){
    delete m_lattice[i];
  }
}

//Copy Constructor makes duplicates of all sites and repoints the neighbor vectors.
Lattice::Lattice(const Lattice &cSource){
  m_R = cSource.m_R;
  m_z = cSource.m_z;
  m_measurements = cSource.m_measurements;

  m_lattice = SiteVect(cSource.m_lattice.size());
  for (unsigned int i = 0 ; i < m_lattice.size() ; i++){
    m_lattice[i] = new Site(*cSource.m_lattice[i]);
  }

  m_site_neighbors = vector<NeighborVect>(m_lattice.size());
  for (unsigned int i = 0 ; i < m_site_neighbors.size() ; i++){
    m_site_neighbors[i] = NeighborVect(cSource.m_site_neighbors[i].size());
    for (unsigned int k = 0 ; k < m_site_neighbors[i].size() ; k++){
      //Point to the same index, but on the new object!
      m_site_neighbors[i][k] = m_lattice[
                                         FindIndexOf(
                                                     cSource.m_lattice, cSource.m_site_neighbors[i][k] )];
    }
  }
}

//This is a copy-paste of the destructor and the copy constructor
Lattice& Lattice::operator=(const Lattice& cSource){
  if (this == &cSource)
    return *this;

  for (unsigned int i = 0 ; i < m_lattice.size() ; i++){
    delete m_lattice[i];
  }

  m_R = cSource.m_R;
  m_z = cSource.m_z;
  m_measurements = cSource.m_measurements;

  m_lattice = SiteVect(cSource.m_lattice.size());
  for (unsigned int i = 0 ; i < m_lattice.size() ; i++){
    m_lattice[i] = new Site(*cSource.m_lattice[i]);
  }

  m_site_neighbors = vector<NeighborVect>(m_lattice.size());
  for (unsigned int i = 0 ; i < m_site_neighbors.size() ; i++){
    m_site_neighbors[i] = NeighborVect(cSource.m_site_neighbors[i].size());
    for (unsigned int k = 0 ; k < m_site_neighbors[i].size() ; k++){
      //Point to the same index, but on the new object!
      m_site_neighbors[i][k] = m_lattice[
                                         FindIndexOf(
                                                     cSource.m_lattice, cSource.m_site_neighbors[i][k] )];
    }
  }

  return *this;
}




void SquareLattice::get_neighbors_init(int site_index, Lattice::NeighborVect* output){
  vector<int> neighbor_coord(2,0);
  (*output) = NeighborVect(4);

  neighbor_coord = IndexToCoord(site_index);
  neighbor_coord[0] += 1;
  (*output)[0] = get_site(neighbor_coord);

  neighbor_coord = IndexToCoord(site_index);
  neighbor_coord[0] -= 1;
  (*output)[1] = get_site(neighbor_coord);

  neighbor_coord = IndexToCoord(site_index);
  neighbor_coord[1] += 1;
  (*output)[2] = get_site(neighbor_coord);

  neighbor_coord = IndexToCoord(site_index);
  neighbor_coord[1] -= 1;
  (*output)[3] = get_site(neighbor_coord);

}




SquareLattice::SquareLattice(Phase default_phase, const vector<int>& sizes, int R, MTRand* rng)
  : Lattice(R, 4, 2, sizes){
  if (m_measurements.size() == 2){
    vector<int> coord(2,0);
    m_lattice = SiteVect(m_measurements[0]*m_measurements[1]);
    for (int i = 0 ; i < m_measurements[0] ; i++){
      for (int j = 0 ; j < m_measurements[1] ; j++){
        coord[0] = i ; coord[1] = j ;
        switch (default_phase){
        case GAS:
          m_lattice[CoordToIndex(coord)] = new Site(m_R, 0.01, 0, .00, rng); break;
        case LIQUID:
          m_lattice[CoordToIndex(coord)] = new Site(m_R, 0.50, 0, .00, rng); break;
        case SOLID:
          m_lattice[CoordToIndex(coord)] = new Site(m_R, 0.99, 0, .00, rng); break;
        case FERRO:
          m_lattice[CoordToIndex(coord)] = new Site(m_R, 0.99, 1, .99, rng); break;
        }
      }
    }
  }
  else if (m_measurements.size() == 0){
    //Do nothing! "zombie" object.
  }
  else{
    throw vector_size_error("Squarelattice::SquareLattice:m_measurements", 2, m_measurements.size());
  }

  if (m_measurements.size() == 2){
    vector<int> coord(2,0);
    m_site_neighbors = vector<NeighborVect>(m_measurements[0]*m_measurements[1]);
    for (int i = 0 ; i < m_measurements[0] ; i++){
      for (int j = 0 ; j < m_measurements[1] ; j++){
        coord[0] = i; coord[1] = j;
        get_neighbors_init(CoordToIndex(coord), &m_site_neighbors[CoordToIndex(coord)]);
      }
    }
  }
}


/*--------------------------------------------------
  Order parameter computation functions
  --------------------------------------------------*/


int SquareLattice::ComputeNOcc(){
  int NOcc = 0;
  vector<int> coord(2);
  Site* currentSite;
  for (int i = 0 ; i < m_measurements[0] ; i++){
    for (int j = 0 ; j < m_measurements[1] ; j++){
      coord[0]=i;   coord[1]=j;
      currentSite = get_site(coord);
      NOcc += currentSite->occ();
    }
  }
  return NOcc;
}


int SquareLattice::ComputeNAligned(int symmetry_num, vector<int>* directions){
  //array for tracking the amount of vector component in each direction
  directions->clear();
  directions->resize(m_R/symmetry_num, 0);
  vector<int> coord(2);
  Site* current_site;
  for (int i = 0 ; i < m_measurements[0] ; i++){
    for (int j = 0 ; j < m_measurements[1] ; j++){
      coord[0]=i;   coord[1]=j;
      current_site = get_site(coord);
      if (current_site->occ()){
        if (current_site->R() == m_R){
          //LHS: Uses only the range of [0, R/symmetry_num-1]
          //RHS: Maps alternating sections to +1 and -1
          (*directions)[current_site->rot() % (m_R/2)] 
            += (((current_site->rot() / (m_R/symmetry_num)) % symmetry_num) * (-2)) + 1;
        }
        else{
          throw container_value_mismatch_error("current_site->R()", current_site->R(), m_R, CoordToIndex(coord));
        }
      }
    }
  }
  return abs(*max_element(directions->begin(), directions->end(), abs_compare));
}


int SquareLattice::ComputeNBond(int symmetry_num){
  int n_bond;
  vector<int> coord(2);
  int index;
  Site* current_site;
  for (int i = 0 ; i < m_measurements[0] ; i++){
    for (int j = 0 ; j < m_measurements[1] ; j++){
      coord[0] = i; coord[1] = j;
      index = CoordToIndex(coord);
      current_site = get_site(index);
      if (current_site->occ()){
        for (unsigned int k = 0 ; k < get_neighbors(index)->size() ; k++){
          if (get_neighbors(index)->at(k)->occ()){
            n_bond += ((get_neighbors(index)->at(k)->rot() % symmetry_num) 
                       ==             (current_site->rot() % symmetry_num)) * 2 - 1;
          }
        }
      }
    }
  }
  //Bonds were double-counted!
  return n_bond/2;
}
 
 
 

 
int TestLatticeCode(){
  MTRand rng;
  vector<int> dimensions;
  dimensions.push_back(5);dimensions.push_back(10);
  SquareLattice l;
  l = SquareLattice(Lattice::LIQUID, dimensions, 96, &rng);
                                               
  vector<int> coord(2,0);
  coord[0] = 2 ; coord[1] = 7;
  
  Site* s = l.get_site(coord);
  Lattice::NeighborVect* n = l.get_neighbors(coord);
  
  l.Print();
  cout << "Site "<< coord[0] <<","<<coord[1]
       <<" has occ: " << s->occ() << " rot: "<<s->rot()<<endl;

  for (unsigned int i = 0; i < n->size() ; i++){
    cout << "Neighbor has occ: " << n->at(i)->occ() << " rot: "<<n->at(i)->rot()<<endl;
    
  }


  s = l.random_site(rng, &n);
  cout << "Random Site"
       <<" has occ: " << s->occ() << " rot: "<<s->rot()<<endl;

  for (unsigned int i = 0; i < n->size() ; i++){
    cout << "Neighbor has occ: " << n->at(i)->occ() << " rot: "<<n->at(i)->rot()<<endl;
    
  }

  return 0;
}
