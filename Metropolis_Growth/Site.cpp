//Site.cpp

#include "Site.h"

//initialize the static variables
int Site::R = 32;
double Site::T = 1.0;

double NemSite::Q = 0;
double NemSite::Q2= 0;
double NemSite::J = 0;

Site::Site(int z_in, double occ_p, MTRand* rng_in, int R_in)
  : z(z_in){

  if (R_in != 0)
    R = R_in; //R = 0 is default value, so as not to change previously set R!


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




double Site::attempt_occ(Site::svec neighbors, double pdel){
  if (!occ)
    set_rot(rand_rot());

  double dE = occ_dE(neighbors);
  dE += chem_potential() * ((occ * 2.0) - 1.0); //chemical potential has a different sign
  double temp_prob = 0;

  if (occ){
    temp_prob = (1/(pdel * R)) * exp(-dE/T);
  }
  if (!occ){
    temp_prob = pdel * R * exp(-dE/T);
  }


  if (rng->rand() < temp_prob){ //canBeOpt
    change_occ();     
    return dE;
  }
  return 0; //if all attempts fail, there is no energy change to report
}

double Site::attempt_occ(Site::svec neighbors, double pdel, double T_in){
  T = T_in;
  return attempt_occ(neighbors, pdel);
}






double Site::attempt_rot(Site::svec neighbors){
  if (occ){
    int plus_minus = ((rng->rand() < .5) * 2) - 1;
    double dE = rot_dE(neighbors, ((rot + plus_minus)+R) % R);
    
    if (rng->rand() < exp(-dE/T)){ //canBeOpt
      move_rot(plus_minus);
      return dE;
    }
  }
  
  return 0;  
}

double Site::attempt_rot(Site::svec neighbors, double T_in){
  T = T_in;
  return attempt_rot(neighbors);
}



/* ---------------------------------------------------------------------
   Nematic Site code
   ---------------------------------------------------------------------*/

NemSite::NemSite(int z_in ,    double occ_p,      MTRand* rng_in,
          double J_in,  double Q_in, double Q2_in,  int R_in)
  : Site(z_in, occ_p, rng_in, R_in){
  J = J_in;
  Q = Q_in;
  Q2=Q2_in;
};



void NemSite::set_interaction(double J_in, double Q_in, double Q2_in){
  J = J_in; 
  Q = Q_in; 
  Q2=Q2_in;
}



double NemSite::curr_interaction(Site* s){
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

    cout << s->get_occ() *(-J - Q * nAlnQ - Q2 * nAlnQ2);

    return s->get_occ() *(-J - Q * nAlnQ - Q2 * nAlnQ2);
  }
                                 
}


double NemSite::curr_interaction(Site::svec sites){
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
  return -J * nOcc - Q * nAlnQ - Q2 * nAlnQ2;
}


//Note: This does not change the rotational state; that must be
//      performed independently!
double NemSite::occ_dE(Site::svec sites){
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


  if (occ)
    return  J * nOcc + Q * nAlnQ + Q2 * nAlnQ2;
  else
    return -J * double(nOcc) - Q * double(nAlnQ) - Q2 * double(nAlnQ2);
  
}

double NemSite::rot_dE(Site::svec sites, int r_try){
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

  return -Q * nAlnQ -Q2 * nAlnQ2;

}

double NemSite::chem_potential(){
  return -J * z / 2.0 - T * log(R);
}
