//Site.cpp
#include "Site.h"



Site::Site(int z_in, double occ_p, MTRand* rng_in, int R_in)
  : z(z_in){

  if (R_in != 0)
    R = R_in;
  else
    R = 16;


  if (rng_in == 0){
    rng = new MTRand();
    rng_new = 1;
  }
  else{
    rng = rng_in;
    rng_new = 0;
  }
  if (rng->rand() < occ_p)
    occ = 1;
  else 
    occ = 0;
  rot = randRot();
}

Site::~Site(){
  if (rng_new) delete rng;
}







/*--------------------------------------------------
  Monte Carlo attempts
  (for any temperature and parameters in)
  --------------------------------------------------*/


double Site::attemptOcc(Site::svec neighbors, double pdel, double T, pvec params, ovec* order, dirtable* directions){
  if (!occ)
    setRot(randRot());

  double dE = occDE(neighbors, params);
  //NOTE :: chemical potential has a different sign depending on the occupation
  dE += chemPotential(T, params) * ((occ * 2.0) - 1.0); 
  double temp_prob = 0;

  if (occ){
    temp_prob = (1/(pdel * R)) * exp(-dE/T);
  }
  if (!occ){
    temp_prob = pdel * R * exp(-dE/T);
  }


  if (rng->rand() < temp_prob){ //canBeOptimized
    changeOcc(order, directions);
    
    return dE;
  }
  return 0; //if all attempts fail, there is no energy change to report
}


double Site::attemptRot(Site::svec neighbors, double T, pvec params, ovec* order, dirtable* directions){
  if (occ){
    int plus_minus = ((rng->rand() < .5) * 2) - 1;
    double dE = rotDE(neighbors, ((rot + plus_minus)+R) % R, params);
    
    if (rng->rand() < exp(-dE/T)){ //canBeOptimized
      moveRot(plus_minus, order, directions);
      return dE;
    }
  }
  
  return 0;  
}


void Site::changeOcc(ovec* order, dirtable* directions){
  changeOcc();

  (*order)[0] += (occ * 2) - 1; //If current occ = 1, add one. If occ = 0, subtract one.
  (*directions)[0][rot % (R/2)] += (occ * 2) - 1;
  (*directions)[1][rot % (R/4)] += (occ * 2) - 1;

  (*order)[1] = abs(*max_element( (*directions)[0].begin(), 
                                  (*directions)[0].end(), 
                                  abs_compare));
                    
  (*order)[2] = abs(*max_element( (*directions)[1].begin(), 
                                  (*directions)[1].end(), 
                                  abs_compare));
}


void Site::moveRot(int plus_minus, ovec* order, dirtable* directions){
  (*directions)[0][rot % (R/2)] -= 1; 
  (*directions)[1][rot % (R/4)] -= 1;
  //cout << "PRE MOVE: "<<rot<<endl;


  moveRot(plus_minus);

  //addition of R to [(rot - plus_minus)] to prevent underflow
  (*directions)[0][rot % (R/2)] += 1;
  (*directions)[1][rot % (R/4)] += 1;
  //cout << "POST MOVE: "<<rot<<endl;

  (*order)[1] = abs(*max_element( (*directions)[0].begin(), 
                                  (*directions)[0].end(), 
                                  abs_compare));
                    
  (*order)[2] = abs(*max_element( (*directions)[1].begin(), 
                                  (*directions)[1].end(), 
                                  abs_compare));

}










/* ---------------------------------------------------------------------
   Nematic Site code
   ---------------------------------------------------------------------*/


double NemSite::currInteraction(Site* s, pvec params){
  if (!occ){
    return 0;
  }
  else{
    if (s->getOcc()){
      //nAln are allowed to be negative to institute the energy penalty
      int alnQ = 0;
      int alnQ2= 0;
      if (s->getRot()==(rot)){
        alnQ  = 1;
        alnQ2 = 1;
      }
      if (s->getRot()==((rot + R/2)%R)){
        alnQ  =-1;
        alnQ2 = 1;
      }
      if (s->getRot()==((rot +  R/4)%R))
        alnQ2 =-1;
      if (s->getRot()==((rot +3*R/4)%R))
        alnQ2 =-1;
      
      //cout << s->getOcc() *(-params[0] - params[1] * nAlnQ - params[2] * nAlnQ2);
      
      return -params[0] - params[1] * alnQ - params[2] * alnQ2;
    }
    else
      return 0;
  }
                                 
}


double NemSite::currInteraction(Site::svec sites, pvec params){

  double dE = 0;

  //Selects a new rotational direction, chosen first to allow for the 
  //neighbor checking step to accurately compute energy difference.
  if (!occ){
    return 0;
  }
  else{
    for (int i=0; i < z; i++){
      dE += currInteraction(sites[i], params);
    }
  }
  return dE;
}


//Note: This does not change the rotational state; that must be
//      performed independently!
double NemSite::occDE(Site::svec sites, pvec params){
  int nOcc = 0;
  //nAln are allowed to be negative to institute the energy penalty
  int nAlnQ = 0;
  int nAlnQ2 = 0;
  
  for (int i=0; i < z; i++){
    if (sites[i]->getOcc()){
      nOcc++;
      if (sites[i]->getRot()==rot){ 
        nAlnQ++; 
        nAlnQ2++; 
      }
      if (sites[i]->getRot()==((rot + R/2)%R)){
        nAlnQ--;
        nAlnQ2++;
      }
      if (sites[i]->getRot()==((rot +  R/4)%R))
        nAlnQ2--;
      if (sites[i]->getRot()==((rot +3*R/4)%R))
        nAlnQ2--;
    }
  }


  if (occ){
    return  params[0] * nOcc + params[1] * nAlnQ + params[2] * nAlnQ2;
  }
  else
    return -params[0] * double(nOcc) - params[1] * double(nAlnQ) - params[2] * double(nAlnQ2);
  
}

double NemSite::rotDE(Site::svec sites, int r_try, pvec params){
  //The output is (E_new - E_old)

  //Negative values are considered "penalties", while positive
  // values are considered "bonuses".
  int nAlnQ = 0;
  int nAlnQ2 = 0;


  if (!occ)
    return 0;
  else{
    for (int i=0; i < z; i++){
      if (sites[i]->getOcc()){
        
        //Original configuration energy:
        if (sites[i]->getRot()==rot){ 
          nAlnQ--; 
          nAlnQ2--; 
        }
        if (sites[i]->getRot()==((rot + R/2)%R)){
          nAlnQ++;
          nAlnQ2--;
        }
        if (sites[i]->getRot()==((rot +  R/4)%R))
          nAlnQ2++;
        if (sites[i]->getRot()==((rot +3*R/4)%R))
          nAlnQ2++;


        //Trial configuration energy (opposite effects!):
        if (sites[i]->getRot()==r_try){
          nAlnQ++;
          nAlnQ2++;
        }
        if (sites[i]->getRot()==((r_try + R/2)%R)){
          nAlnQ--;
          nAlnQ2++;
        }
        if (sites[i]->getRot()==((r_try +  R/4)%R))
          nAlnQ2--;
        if (sites[i]->getRot()==((r_try +3*R/4)%R))
          nAlnQ2--;
      }
    }
  }

  return -params[1] * nAlnQ - params[2] * nAlnQ2;

}

double NemSite::chemPotential(double T, pvec params){
  return -params[0] * z / 2.0 - T * log(R);
}
