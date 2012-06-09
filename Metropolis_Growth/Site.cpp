//Site.cpp
#include "Site.h"

Site::ovec Site::dOrder = Site::ovec(3);



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
  rot = rand_rot();
}

Site::~Site(){
  if (rng_new) delete rng;
}













/*--------------------------------------------------
  Monte Carlo attempts
  (for any temperature and parameters in)
  --------------------------------------------------*/


double Site::attempt_occ(Site::svec neighbors, double pdel, double T, pvec params, ovec* order){
  if (!occ)
    set_rot(rand_rot());

  double dE = occ_dE(neighbors, params);
  //NOTE :: chemical potential has a different sign depending on the occupation
  dE += chem_potential(T, params) * ((occ * 2.0) - 1.0); 
  double temp_prob = 0;

  if (occ){
    temp_prob = (1/(pdel * R)) * exp(-dE/T);
  }
  if (!occ){
    temp_prob = pdel * R * exp(-dE/T);
  }


  if (rng->rand() < temp_prob){ //canBeOptimized
    change_occ();     
    (*order)[0] += dOrder[0];
    (*order)[1] += dOrder[1];
    (*order)[2] += dOrder[2];
    
    return dE;
  }
  return 0; //if all attempts fail, there is no energy change to report
}

double Site::attempt_rot(Site::svec neighbors, double T, pvec params, ovec* order){
  if (occ){
    int plus_minus = ((rng->rand() < .5) * 2) - 1;
    double dE = rot_dE(neighbors, ((rot + plus_minus)+R) % R, params);
    
    if (rng->rand() < exp(-dE/T)){ //canBeOptimized
      move_rot(plus_minus);
      (*order)[0] += dOrder[0];
      (*order)[1] += dOrder[1];
      (*order)[2] += dOrder[2];
        
      return dE;
    }
  }
  
  return 0;  
}
















/* ---------------------------------------------------------------------
   Nematic Site code
   ---------------------------------------------------------------------*/


double NemSite::curr_interaction(Site* s, pvec params){
  if (!occ){
    return 0;
  }
  else{
    int nAlnQ = 0;
    int nAlnQ2= 0;
    if (s->get_rot()==((rot + R/2)%R)){
      nAlnQ--;
      nAlnQ2++;
    }
    if (s->get_rot()==((rot +  R/4)%R))
      nAlnQ2--;
    if (s->get_rot()==((rot +3*R/4)%R))
      nAlnQ2--;

    //cout << s->get_occ() *(-params[0] - params[1] * nAlnQ - params[2] * nAlnQ2);

    return s->get_occ() *(-params[0] - params[1] * nAlnQ - params[2] * nAlnQ2);
  }
                                 
}


double NemSite::curr_interaction(Site::svec sites, pvec params){
  int nOcc = 0;
  //nAln are allowed to be negative to institute the energy penalty
  int nAlnQ = 0;
  int nAlnQ2 = 0;

  //Selects a new rotational direction, chosen first to allow for the 
  //neighbor checking step to accurately compute energy difference.
  if (!occ){
    return 0;
  }
  else{
    for (int i=0; i < z; i++){
      if (sites[i]->get_occ()){
        nOcc++;
        if (sites[i]->get_rot()==rot){ 
          nAlnQ++; 
          nAlnQ2++; 
        }
        if (sites[i]->get_rot()==((rot + R/2)%R)){
          nAlnQ--;
          nAlnQ2++;
        }
        if (sites[i]->get_rot()==((rot +  R/4)%R))
          nAlnQ2--;
        if (sites[i]->get_rot()==((rot +3*R/4)%R))
          nAlnQ2--;
      }
    }
  }
  return -params[0] * nOcc - params[1] * nAlnQ - params[2] * nAlnQ2;
}


//Note: This does not change the rotational state; that must be
//      performed independently!
double NemSite::occ_dE(Site::svec sites, pvec params){
  int nOcc = 0;
  //nAln are allowed to be negative to institute the energy penalty
  int nAlnQ = 0;
  int nAlnQ2 = 0;
  
  for (int i=0; i < z; i++){
    if (sites[i]->get_occ()){
      nOcc++;
      if (sites[i]->get_rot()==rot){ 
        nAlnQ++; 
        nAlnQ2++; 
      }
      if (sites[i]->get_rot()==((rot + R/2)%R)){
        nAlnQ--;
        nAlnQ2++;
      }
      if (sites[i]->get_rot()==((rot +  R/4)%R))
        nAlnQ2--;
      if (sites[i]->get_rot()==((rot +3*R/4)%R))
        nAlnQ2--;
    }
  }


  if (occ){
    dOrder[0] = -1;
    dOrder[1] = -nAlnQ/2.0;
    dOrder[2] = -nAlnQ2/2.0;
    return  params[0] * nOcc + params[1] * nAlnQ + params[2] * nAlnQ2;
  }
  else
    dOrder[0] = 1;
    dOrder[1] = nAlnQ/2.0;
    dOrder[2] = nAlnQ2/2.0;
    return -params[0] * double(nOcc) - params[1] * double(nAlnQ) - params[2] * double(nAlnQ2);
  
}

double NemSite::rot_dE(Site::svec sites, int r_try, pvec params){
  //The output is (E_new - E_old)

  //Negative values are considered "penalties", while positive
  // values are considered "bonuses".
  int nAlnQ = 0;
  int nAlnQ2 = 0;

  //Selects a new rotational direction, chosen first to allow for the 
  //neighbor checking step to accurately compute energy difference.

  if (!occ)
    return 0;
  else{
    for (int i=0; i < z; i++){
      if (sites[i]->get_occ()){
        
        //Original configuration energy:
        if (sites[i]->get_rot()==rot){ 
          nAlnQ--; 
          nAlnQ2--; 
        }
        if (sites[i]->get_rot()==((rot + R/2)%R)){
          nAlnQ++;
          nAlnQ2--;
        }
        if (sites[i]->get_rot()==((rot +  R/4)%R))
          nAlnQ2++;
        if (sites[i]->get_rot()==((rot +3*R/4)%R))
          nAlnQ2++;


        //Trial configuration energy (opposite effects!):
        if (sites[i]->get_rot()==r_try){
          nAlnQ++;
          nAlnQ2++;
        }
        if (sites[i]->get_rot()==((r_try + R/2)%R)){
          nAlnQ--;
          nAlnQ2++;
        }
        if (sites[i]->get_rot()==((r_try +  R/4)%R))
          nAlnQ2--;
        if (sites[i]->get_rot()==((r_try +3*R/4)%R))
          nAlnQ2--;
      }
    }
  }

  dOrder[0] = 0;
  dOrder[1] = nAlnQ/2.0;
  dOrder[2] = nAlnQ2/2.0;
  return -params[1] * nAlnQ - params[2] * nAlnQ2;

}

double NemSite::chem_potential(double T, pvec params){
  return -params[0] * z / 2.0 - T * log(R);
}
