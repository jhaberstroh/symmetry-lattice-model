#include "orderparam.h"
#include "Site.h"


void deltaDir::initOP(Lattice const& l){
  rho = 0;
  n1 = 0;
  n2 = 0;
  n1Tally = vector<int>( l.getR()/n1 , 0);
  n2Tally = vector<int>( l.getR()/n2 , 0);

  

  Site* currentSite;
  int currentRot;

  for (int i = 0 ; i < latSize ; i++){
    currentSite = l.getSite(i);
    nghbors = pullNeighbors(i);
    if (currentSite->getOcc()){
      rho += 1;
      currentRot = currentSite->getRot();

      n1Tally[currentRot % n1Tally.size()] = (((currentRot / n1Tally.size()) % 2) * (-2)) + 1;
      n2Tally[currentRot % n2Tally.size()] = (((currentRot / n2Tally.size()) % 2) * (-2)) + 1;
    }
  }

  n1 = abs(*max_element(n1Tally.begin(), n1Tally.end(), abs_compare));
  n2 = abs(*max_element(n2Tally.begin(), n2Tally.end(), abs_compare));

}

void deltaDir::updateOP(ptype op, vector<int> mod){
  switch (op){

  case OCC:

    //When OCC, mod should have the following format:
    // mod[0]: Change in number of occupiers (integer, and for metropolis, only +1 or -1)
    rho += mod[0];
    break;



  case N1:

    //When N1, mod should have the following format:
    // mod[0]: Change in occupation (+1, -1, or 0. 
    //                               If 0, it was an occupation move. 
    //                               If nonzero, it was a rotation move)
    // mod[1]: Rotational state after change (site.rot) (integer)
    // mod[2]: Direction of change (plus_minus) (integer) (subtract from newR to get oldR)
    // mod[3]: Change in favorable bonds (integer)

    if (mod[0] == 0){

      //----ROTATIONAL MOVE----
      //oldR = (mod[1]-mod[2]+R)%R
      //newR = mod[1]

      n1Tally[((mod[1]-mod[2]+R)%R)%n1] -= (((mod[1]-mod[2]+R)%R) /n1) %2;
      n1Tally[          mod[1]     %n1] -=      (     mod[1]      /n1) %2;
    }
    else if (mod[0] == 1 or mod[0] == -1){
      //----OCCUPATIONAL MOVE----
      n1Tally[ mod[1]%n1 ] += mod[0] * (mod[1]/n1) %2;
    }
    else{
      cout<<"Problem updating order parameters, in deltaDir::updateOP/switch(op)/caseN1/mod[0], bad value for mod[0] (should only be +1/0/-1)"<<endl;
    }
    break;



  case N2:

    // see updateOP/case N1 (line 42) for formatting rules.
    if (mod[0] == 0){

      //----ROTATIONAL MOVE----
      //oldR = (mod[1]-mod[2]+R)%R
      //newR = mod[1]

      n2Tally[((mod[1]-mod[2]+R)%R)%n2] -= (((mod[1]-mod[2]+R)%R) /n2) %2;
      n2Tally[          mod[1]     %n2] -=      (     mod[1]      /n2) %2;
    }
    else if (mod[0] == 1 or mod[0] == -1){
      //----OCCUPATIONAL MOVE----
      n2Tally[ mod[1]%n2 ] += mod[0] * (mod[1]/n2) %2;
    }
    else{
      cout<<"Problem updating order parameters, in deltaDir::updateOP/switch(op)/caseN2/mod[0], bad value for mod[0] (should only be +1/0/-1)"<<endl;
    }
    break;
  }
}







double bondCount::initOP(Lattice const& l){
 rho = 0;
  n1 = 0;
  n2 = 0;

  

  Site* currentSite;
  Site::svec nghbors;
  int currentRot;

  for (int i = 0 ; i < latSize ; i++){
    currentSite = l.getSite(i);

    if (currentSite->getOcc()){
      rho += 1;


      currentRot = currentSite->getRot();
      nghbors = pullNeighbors(i);
      for (int j = 0 ; i < nghbors.size() ; i++){
        //only check for orientation alignment if both sites are occupied!
        if (nghbors[j]->getOcc()){

          if (nghbors[j]->getRot%n1 == currentRot%n1)
            n1 += abs( (nghbors[j]/n1)
                      -(currentRot/n1) ) % 2;

          if (nghbors[j]->getRot%n2 == currentRot%n2)
            n2 += abs( (nghbors[j]/n2)
                      -(currentRot/n2) ) % 2;

        }
      }
    }
  }
}


void bondCount::updateOP(ptype op, vector<int> mod){
  switch (op){
    
  case OCC:

    //When OCC, mod should have the following format:
    // mod[0]: Change in number of occupiers (integer, and for metropolis, only +1 or -1)
    rho += mod[0];
    break;


  case N1:
 
    // see updateOP/case N1 (line 42) for formatting rules.
    n1 += mod[3];
    break;


  case N2:

    // see updateOP/case N1 (line 42) for formatting rules.
    n2 += mod[3];
    break;
  }
}
