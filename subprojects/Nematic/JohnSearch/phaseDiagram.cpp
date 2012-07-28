#include "Bead.h"



int main(int argc, char* argv){
  double jmin, qmin, q2min;
  double jmax, qmax, q2max;
  int resolution;


  if (argc == 1){
    char input;
    cuot << "Run default? Type y if yes, and other for usage.\n";
    cin >> input;
    if (input == 'y' || input == 'Y'){
      jmin = 0.1;
      jmax = 3.1;
      qmin = 0.1;
      qmax = 3.1;
      q2min = 0.1;
      q2max = 3.1;
      resolution = 20;
    }
    else{
      cout << "program aborted.\n"
      cout << "USAGE: ./phaseDiagram.exe (resolution) (jmin) (jmax) (qmin) (qmax) (q2min) (q2max)\n";
      return 1;
    }
  }

  else if (argc == 7){
    //Parse input
    return 1;
  }
  
  else{
    cout << "program aborted.\n"
    cout << "USAGE: ./phaseDiagram.exe (resolution) (jmin) (jmax) (qmin) (qmax) (q2min) (q2max)\n";
    return 1;
  }
  
  for (int i = 0; i < resolution; i++){
    for (int j = 0; j < resolution; j++){
      for (int k = 0; k < resolution; k++){



      }
    }
  }

   



}
