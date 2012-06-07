#include "Bead.h"

int main(int argc, char** argv){
  double T;
  int R;
  double dt;
  int steps;

  double J;
  double Q;
  double Q2;
  
  double phi0;
  double tau0;
  double omega0;
  
  bool fail = 0;


  /* ----------------------------------------
  /  Robust variable sized input (start)
  /---------------------------------------- */

  if (argc < 2 || 
      sscanf(argv[1],"%d",&steps) != 1){
    fail = 1;
  }
  if (argc >= 3 && !fail){
    if (sscanf(argv[2], "%lf", &dt) != 1){
      fail = 1;
    }
  }


  else dt = .01;
  if (argc >= 4 && !fail){
    if (sscanf(argv[3], "%lf", &J) != 1){
      fail = 1;
    }
  }
  else J = 1.0;
  if (argc >= 5 && !fail){
    if (sscanf(argv[4], "%lf", &Q) != 1){
      fail = 1;
    }
  }
  else Q = 12.5;
  if (argc >= 6 && !fail){
    if (sscanf(argv[5], "%lf", &Q2)!= 1){
      fail = 1;
    }
  }
  else Q2 = 20.0;



  if (argc >= 7 && !fail){
    if (sscanf(argv[6], "%lf", &T) != 1){
      fail = 1;
    }
  }
  else T = 1.0;
  if (argc >= 8 && !fail){
    if (sscanf(argv[7], "%d", &R)  != 1){
      fail = 1;
    }
  }
  else R = 32;


  if (argc >= 9 && !fail){
    if (sscanf(argv[8], "%lf", &phi0)  != 1){
      fail = 1;
    }
  }
  else phi0 = .001;
  if (argc >= 10 && !fail){
    if (sscanf(argv[9], "%lf", &tau0)  != 1){
      fail = 1;
    }
  }
  else tau0 = .001;
  if (argc >= 11&& !fail){
    if (sscanf(argv[10], "%lf", &omega0)  != 1){
      fail = 1;
    }
  }
  else omega0 = .001;

  if (fail){
    cout << printf("Use the following format: ./beads.o steps (dt) (J) (Q) (Q2) (T) (R) (phi0) (tau0) (omega0)\n **Note: the arguments in parentheses are optional.\n");
    exit(1);
  }
    
  /* ----------------------------------------
  /  Robust variable sized input (end)
  /---------------------------------------- */




  Bead b (phi0, tau0, omega0, dt);

  b.changeTrackSet(T,R,J,Q,Q2);

  for (int i = 0 ; i < steps ; i++){
    b.heun_step(T,R,J,Q,Q2);
  }

  b.cout_phase(b.decide_phase());
  return 0;

}

