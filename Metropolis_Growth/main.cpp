//main.cpp
#include "monteCarlo.h"



int main(int argc, char* argv){
  MonteCarlo m;

  m.set_j(0);

  for (int i = 0 ; i < 10 ; i++){
    m.DoMetropolisSweep();
    m.PrintLattice();
  }

  cout << "Changing parameters! \n\n\n\n";
  
  m.set_j(3);

  for (int i = 0 ; i < 10 ; i++){
    m.DoMetropolisSweep();
    m.PrintLattice();
  }

}
