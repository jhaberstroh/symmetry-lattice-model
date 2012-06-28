//main.cpp
#include "monteCarlo.h"

void DoNSweepsAtJ(MonteCarlo& m, int N, int J){
  m.set_j(J);
  for (int i = 0 ; i < N ; i++){
    m.DoMetropolisSweep();
  }
  m.PrintLattice();
}

int main(int argc, char* argv){
  MonteCarlo m;

  m.set_qN1(0);
  m.set_qN2(0);

  m.PrintLattice();
  m.TestNeighborPointers();


  for (double J = 1 ; J < 3.0 ; J += .05){
    DoNSweepsAtJ(m, 10, J);
    cout << "Changing parameters! \n\n\n\n";
  }


}
