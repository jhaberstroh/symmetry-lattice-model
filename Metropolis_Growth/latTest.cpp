#include "Lattice.h"

int main(){
  vector<int> size(2);

  //Usage:
  //SquareLattice(J,Q,Q2,R,T,pdel, MTRand* rng_in, vector<int> sizes,
  //              Phase p, Interaction itr)
  vector<int> siz(2);
  siz[0] = 15;
  siz[1] = 15;
  SquareLattice wowza( 3.4, .2, 1.77, 
                       24, 1,
                       .1, 0, siz);
  cout<< "Initial Energy: "<<wowza.getE()<<"\n";
  cout<< "Initial Phi: "<<wowza.get_phi()<<endl;
  cout<< "Initial Tau: "<<wowza.get_tau()<<endl;
  cout<< "Initial Omega: "<<wowza.get_omega()<<endl;

  
  //can run 3million without much difficulty
  for (int i = 0 ; i < 300000 ; i++){
    wowza.metro_move();
  }

  cout<< "Final Energy: "<<wowza.getE()<<endl;
  cout<< "Final Phi: "<<wowza.get_phi()<<endl;
  cout<< "Final Tau: "<<wowza.get_tau()<<endl;
  cout<< "Final Omega: "<<wowza.get_omega()<<endl;
  wowza.printLat();

  return 0;
}
