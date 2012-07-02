//main.cpp
#include "monteCarlo.h"

void DoNSweepsAtJ(MonteCarlo& m, int N, int J){
  m.set_j(J);
  for (int i = 0 ; i < N ; i++){
    m.DoMetropolisSweep();
  }
  m.PrintLattice();
  m.PrintOrderParameters();
  m.Track();
}

int main(int argc, char* argv){
  MonteCarlo m;

  m.set_qN1(3);
  m.set_qN2(0);
  m.reset_default_phase(Lattice::LIQUID);

  m.PrintLattice();
  m.TestNeighborPointers();


  m.SetupTrack();
  m.Track();
  for (double J = 1.0 ; J < 3.0 ; J += .4){
    cout << "Changing parameters! \n\n\n\n";
    DoNSweepsAtJ(m, 10, J);
  }

  m.ResetOP();
  m.PrintOrderParameters();

}
