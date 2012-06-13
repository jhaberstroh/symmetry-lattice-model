#include "Lattice.h"
#include "Site.h"

int main(int argc, char** argv){
  double startQ, stopQ;
  int annSteps, nSweeps;
  clock_t t1, t2;
  bool fail = 0;

  vector<int> siz(2);
  siz[0] = 10;
  siz[1] = 10;
  int n_sites = siz[0] * siz[1];


  //Required arguments
  if (argc < 4 || 
      sscanf(argv[1],"%lf",&startQ) != 1 ||
      sscanf(argv[2],"%lf",&stopQ)  != 1 ||
      sscanf(argv[3],"%d",&annSteps)!= 1){
    fail = 1;
  }

  //Optional arguments
  if (argc >= 5 && !fail){
    if (sscanf(argv[3], "%d", &nSweeps) != 1){
      fail = 1;
    }
  }
  else{
    nSweeps = 30000;
  }

  if (fail){
    cout << "ERROR & EXIT: Usage is wrong. Correct usage is:\n\n\n"
         << "\t./anneal.exe startQ stopQ annSteps (nSweeps)\n\n\n"
         << "NOTE: \t() options are optional." << endl;
    return 1;
  }

  //Usage:
  //SquareLattice(J,Q,Q2,R,T,pdel, MTRand* rng_in, vector<int> sizes,
  //              Phase p, Interaction itr)
  
  Site::pvec params(3);
  params[0] = 0;
  params[1] = startQ;
  params[2] = .3;

  //Because pdel is set to 1, there are no rotational moves, and so this is exactly
  //reminiscent of the ising model
  SquareLattice wowza( params, 4, 
                       1,.000000000000000000001, 0, siz);



  //------------------------------Operations------------------------------








  cout<< "Initial Energy: "<<wowza.getE()<<"\n";
  cout<< "Initial Phi: "<<wowza.getPhi()<<endl;
  cout<< "Initial Tau: "<<wowza.getTau()<<endl;
  cout<< "Initial Omega: "<<wowza.getOmega()<<endl;

  vector<double> spHeat(annSteps, 0);
  double phiSquaredAvg;
  double phiAvgAvg;
 
  
  wowza.optimize();
  t1 = clock();

  int a = nSweeps/10;
  int b = n_sites * 10 - 1;
  double stepSizeQ = (stopQ - startQ)/(annSteps - 1);

  params[1] -= stepSizeQ; //Hoakey fix to interpolate between startQ and stopQ

  for (int k = 0 ; k < annSteps ; k ++){
    params[1] += stepSizeQ;
    wowza.setParams(params);
    phiSquaredAvg = 0;
    phiAvgAvg = 0;

    for (int i = 0 ; i < a ; i++){
      wowza.optMetroMove();
      for (int j = 0 ; j < b ; j++){
        wowza.optMetroMove();
        ///        phiSquaredAvg += wowza.getPhi_sq()/n_sites/(nSweeps * n_sites);
        //        phiAvgAvg += wowza.getPhi_macro()/(nSweeps * n_sites);
      }
    }
    wowza.printLat();
    spHeat[k] = wowza.getOmega()-wowza.getTau();
  }


  for (int i  = 0; i < annSteps ; i++){
    cout << "At Q = " << startQ << ", nematic gap is: " << spHeat[i] << endl;
    startQ += stepSizeQ;
  }







  t2 = clock();
  float diff((float)t2 - (float) t1);
  cout << "To run optimized for "<<10*a*(b+1)<<" moves, it took: "<< diff/CLOCKS_PER_SEC <<"s"<<endl;
  
  


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
  wowza.printLat();  


  cout<< "Final Energy: "<<wowza.getE()<<endl;
  cout<< "Final Phi: "<<wowza.getPhi()<<endl;
  cout<< "Final Tau: "<<wowza.getTau()<<endl;
  cout<< "Final Omega: "<<wowza.getOmega()<<endl;

  wowza.resetOP();

  cout<< "Final Energy: "<<wowza.getE()<<endl;
  cout<< "Final Phi: "<<wowza.getPhi()<<endl;
  cout<< "Final Tau: "<<wowza.getTau()<<endl;
  cout<< "Final Omega: "<<wowza.getOmega()<<endl;


  return 0;
  





}
