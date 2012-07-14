//main.cpp
#include "montecarlo.h"

void DoNSweepsAtJ(MonteCarlo& m, int N, int J){
  m.set_j(J);
  for (int i = 0 ; i < N ; i++){
    m.DoMetropolisSweep();
  }
  //  m.PrintLattice();
  //  m.PrintOrderParameters();
  m.file_handler().Track();
}

int main(int argc, char** argv){
  MonteCarlo m;

  m.set_qN1(3);
  m.set_qN2(0);
  m.reset_default_phase(Lattice::LIQUID);

  //  m.PrintLattice();

  m.file_handler().Track();
  for (double J = 1.0 ; J < 3.0 ; J += .4){
    cout << "Changing parameters! \n\n\n\n";
    DoNSweepsAtJ(m, 10, J);
  }

  m.PrintOrderParameters();
  m.file_handler().MakeOPImage();
}
