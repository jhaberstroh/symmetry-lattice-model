//Lattice.cpp
#include "lattice.h"

using namespace std;

/*--------------------------------------------------
  Indexing functions & utilities
  --------------------------------------------------*/

int FindIndexOf(vector<Site*> array, Site* s){
  for (unsigned int i = 0 ; i < array.size() ; i++){
    if (s == array[i])
      return i;
  }

  return -1 + abs_compare(1,0); //sentinel return value
}




vector<int> Lattice::view_site(int site_index){
  vector<int> r_values(1 + m_site_neighbors[site_index].size(),0);
  //uses the conditional operator to assign the sentinel value
  // of -1 if a site is not occupied!
  r_values[0] = (m_lattice[site_index]->occ())?
                 m_lattice[site_index]->R() : -1;
  for (int i = 0 ; i < m_z ; i++){
    r_values[i+1] = (m_site_neighbors[site_index][i]->occ())?
                     m_site_neighbors[site_index][i]->R() : -1;
  }
  return r_values;
}


string Lattice::PhaseStringLookup(Lattice::Phase p){
    switch (p){
        case GAS:
            return "Gas"; break;
        case LIQUID:
            return "Liquid"; break;
        case SOLID:
            return "Solid"; break;
        case FERRO:
            return "Ferro"; break;
        default:
            return "Bad Selection"; break;
    }
}

Lattice::Phase Lattice::IntToPhase(int p){
    switch (p){
        case GAS:
            return Lattice::GAS; break;
        case LIQUID:
            return Lattice::LIQUID; break;
        case SOLID:
            return Lattice::SOLID; break;
        case FERRO:
            return Lattice::FERRO; break;
        default:
            return Lattice::N_PHASES; break;
    }
}

/*--------------------------------------------------
  Constructors, Destructors, Assignment (Non-trivial!)
  --------------------------------------------------*/

Lattice::~Lattice(){
  for (unsigned int i = 0 ; i < m_lattice.size() ; i++){
    delete m_lattice[i];
  }
  delete m_lattice_handler;
}

//Copy Constructor makes duplicates of all sites and repoints the neighbor vectors.
Lattice::Lattice(const Lattice &cSource){
    //just to make the compiler stop complaining about abs_compare
  if (abs_compare(1,2)){
    m_R = cSource.m_R;
    m_z = cSource.m_z;
    m_measurements = cSource.m_measurements;
  }

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
  m_lattice_handler = new LatticeFile(*this);
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

  //m_lattice_handler still points to the appropriate Lattice object ("this"), so
  // it does not need to be deleted and re-instantiated!
  return *this;
}



/*--------------------------------------------------
  Square Lattice Functions
  --------------------------------------------------*/


SquareLattice::SquareLattice(Phase default_phase, const vector<int>& sizes, int R, MTRand* rng)
  : Lattice(R, 4, 2, sizes){
  if (m_measurements.size() == 2){
    Coord coord(2,0);
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
        default:
          //Use Liquid as the default
          cout << "Problem in SquareLattice constructor: inappropriate default phase selected.\n"
               << "Using LIQUID as the default phase." << endl;
          m_lattice[CoordToIndex(coord)] = new Site(m_R, 0.50, 0, .00, rng); break;
        }
      }
    }

    m_site_neighbors = vector<NeighborVect>(m_measurements[0]*m_measurements[1]);
    for (int i = 0 ; i < m_measurements[0] ; i++){
      for (int j = 0 ; j < m_measurements[1] ; j++){
        coord[0] = i; coord[1] = j;
        InitializeNeighborVector(CoordToIndex(coord), &(m_site_neighbors[CoordToIndex(coord)]));
      }
    }
  }

  else if (m_measurements.size() == 0){
    //Do nothing! "zombie" object.
  }
  else{
    throw vector_size_error("Squarelattice::SquareLattice:m_measurements", 2, m_measurements.size());
  }
}


void SquareLattice::InitializeNeighborVector(int site_index, Lattice::NeighborVect* output){
  //Order is down,
  Coord neighbor_coord(2,0);
  (*output) = NeighborVect(4);

  (*output)[kNeighUp]    = get_site(GetNeighborCoord(IndexToCoord(site_index), kNeighUp));
  (*output)[kNeighDown]  = get_site(GetNeighborCoord(IndexToCoord(site_index), kNeighDown));
  (*output)[kNeighLeft]  = get_site(GetNeighborCoord(IndexToCoord(site_index), kNeighLeft));
  (*output)[kNeighRight] = get_site(GetNeighborCoord(IndexToCoord(site_index), kNeighRight));
}


Lattice::BondVect SquareLattice::CreateBondVector(){
  return BondVect(m_lattice.size() * 2, 0);
}


//CoordToIndex treats the input coordinates completely periodically;
// thus, it will not be the highest performance if input value is bounded,
// but it is extremely robust (great for readability).
int SquareLattice::CoordToIndex(const Coord& coord){
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


Lattice::Coord SquareLattice::IndexToCoord(int index){
  index = ((index % m_lattice.size()) +m_lattice.size()) %m_lattice.size();
  Coord coord_out(2,0);

  coord_out[0] = index / m_measurements[1];
  coord_out[1] = index % m_measurements[1];

  return coord_out;
}


int SquareLattice::LookupBondIndex(const Coord& coord, int direction){
  /* Bonds are specified as follows:

     0_ 00 _1_ 02 _2_ 04 _3_ 06

    01     03     05     07

     4_ 08 _5_ 10 _6_ 12 _7_ 14

    09     11     13     15

   */

  if (!(direction < m_z)){
    throw bad_direction("SquareLattice::LookupBondIndex:direction", direction, m_z);
  }
  if (coord.size() != 2)
    throw vector_size_error("SquareLattice::LookupBondIndex:coord", 2, coord.size());
  else{
    switch (direction){
    case kNeighUp:
      return CoordToIndex(GetNeighborCoord(coord, kNeighUp)) * 2 + 1;
      break;
    case kNeighDown:
      return CoordToIndex(coord) * 2 + 1;
      break;
    case kNeighLeft:
      return CoordToIndex(GetNeighborCoord(coord, kNeighLeft)) * 2 + 0;
      break;
    case kNeighRight:
      return CoordToIndex(coord) * 2 + 0;
      break;
    }
  }
  return -1;//should never reach here, but the compiler is whiny, and who knows, maybe there will be terrible errors
}

Lattice::Coord SquareLattice::GetNeighborCoord(const Coord& coord, int direction){
  if (! (direction < m_z))
    throw bad_direction("SquareLattice::LookupBondIndex:direction", direction, m_z);

  if (coord.size() == 2){
    //The input for neighborIndex is restricted by the Enum type.
    Coord neighbor_coord = coord;
    switch (direction){
    case kNeighUp:
      neighbor_coord[0] -= 1;
      break;
    case kNeighDown:
      neighbor_coord[0] += 1;
      break;
    case kNeighLeft:
      neighbor_coord[1] -= 1;
      break;
    case kNeighRight:
      neighbor_coord[1] += 1;
      break;
    }
    return neighbor_coord;
  }
  else{
    throw vector_size_error("SquareLattice::GetNeighborCoord", 2, coord.size());
  }
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
  Coord coord(2);
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


int TestLatticeCode(){
  MTRand rng;
  vector<int> dimensions;
  dimensions.push_back(5);dimensions.push_back(10);
  SquareLattice l;
  l = SquareLattice(Lattice::LIQUID, dimensions, 96, &rng);

  Lattice::Coord coord(2,0);
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
