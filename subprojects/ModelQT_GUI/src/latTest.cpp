#include <time.h>
#include "Lattice.h"

int main(){
  clock_t t1, t2;
  int sweeps = 10000;

  //Usage:
  //SquareLattice(J,Q,Q2,R,T,pdel, MTRand* rng_in, vector<int> sizes,
  //              Phase p, Interaction itr)
  vector<int> siz(2);
  siz[0] = 15;
  siz[1] = 15;
  int n_sites = siz[0] * siz[1];


  SquareLattice wowza( 3.4, .2, 1.77, 
                       24, 1,
                       .1, 0, siz);
  cout<< "Initial Energy: "<<wowza.getE()<<"\n";
  cout<< "Initial Phi: "<<wowza.get_phi()<<endl;
  cout<< "Initial Tau: "<<wowza.get_tau()<<endl;
  cout<< "Initial Omega: "<<wowza.get_omega()<<endl;

  
  //can run 3million without much difficulty
  wowza.optimize();
  t1 = clock();

  for (int i = 0 ; i < sweeps ; i++){
    wowza.t_opt_metro_move();
    for (int j = 0 ; j < n_sites-1 ; j++){
      wowza.opt_metro_move();
    }
  }


  t2 = clock();
  float diff((float)t2 - (float) t1);
  cout << "To run optimized for "<<sweeps*n_sites<<" moves, it took: "<< diff/CLOCKS_PER_SEC <<"s"<<endl;
  
  


  /*
  SquareLattice cowza( 3.4, .2, 1.77, 
                       24, 1,
                       .1, 0, siz);
  t1 = clock();
  for (int i = 0 ; i < moves ; i++){
    cowza.metro_move();
  }
  t2 = clock();
  diff = float((float)t2 - (float) t1) ;  
  cout << "To run regular for "<<moves<<" moves, it took: "<< diff/CLOCKS_PER_SEC <<"s"<<endl;

  */


  cout<< "Final Energy: "<<wowza.getE()<<endl;
  cout<< "Final Phi: "<<wowza.get_phi()<<endl;
  cout<< "Final Tau: "<<wowza.get_tau()<<endl;
  cout<< "Final Omega: "<<wowza.get_omega()<<endl;
  wowza.printLat();

  return 0;
}
