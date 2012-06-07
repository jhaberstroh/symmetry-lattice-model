#include <iostream>
#include "Site.h"

int main(void){
  Site* tester1 = new NemSite(4, 0, 0, .563, 1, .23, 24);

  Site* tA = new NemSite(1, .7, 0);
  Site* tB = new NemSite(1, .7, 0);
  Site* tC = new NemSite(1, .7, 0);
  Site* tD = new NemSite(1, .7, 0);

  Site* x[] = {tA, tB, tC, tD};

  for (int i = 0; i < 20 ; i++){
    cout << "ENERGY CHANGE: " << tester1->attempt_occ(x, .5) << endl;
    cout << "ENERGY CHANGE:  "<< tester1->attempt_rot(x) << endl;
    //cout << "Occupation: " << tester1->get_occ() << "\n";
    //cout << "Rotation: " << tester1->get_rot() << "\n";
  }



  delete tester1;
  delete tA;
  delete tB;
  delete tC;
  delete tD;

}
