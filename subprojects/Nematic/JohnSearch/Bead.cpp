/*--------------------------------------------------------------------------
** This Beads class implements heun integration in a three dimensional
** parameter space. It is currently configured to run analysis of the
** mean field theory free energy of the 2-D ferromagnetic/nematic/liquid system.
  --------------------------------------------------------------------------*/


#include <cmath>
#include <iostream>
#include <fstream>
#include <sstream>
#include "Bead.h"

using namespace std;


//CLASS CONSTRUCTORS:
Bead::Bead(){
  p = .01;
  t = .01;
  o = .01;
  dt = .01;

  p0 = p;
  t0 = t;
  o0 = o;
  
  pF = tF = oF = 0; //replace with compute forces?
  e = 0;            //replace with compute energy?
  track = 0;

  mob_p = mob_t = mob_o = 1;
  
  //    filename.setf(ios::fixed, ios::floatfield);
  //    filename.precision(4);
  //    filename.str("");

}
  
Bead::Bead(double phi, double tau, double omega, double timestep){
    p = phi;
    t = tau;
    o = omega;
    dt = timestep;

    p0 = p;
    t0 = t;
    o0 = o;
    
    pF = tF = oF = 0; //replace with compute forces?
    e = 0;            //replace with compute energy?
    track = 0;

    mob_p = mob_t = mob_o = 1;
  
    //    filename.setf(ios::fixed, ios::floatfield);
    //    filename.precision(4);
    //    filename.str("");
  }


//overloaded function; can call with either type of input
//Intended to come with default values
Bead::Bead(Order_Param t1, double v1, 
           Order_Param t2, double v2, 
           Order_Param t3, double v3,
           double timestep){
  //set default values
  p=t=o=.02;
  switch(t3)
    {
    case PHI:  p=v3;break;
    case TAU:  t=v3;break;
    case OMEGA:o=v3;break;
    default :  break;
    }
  switch(t2)
    {
    case PHI:  p=v2;break;
    case TAU:  t=v2;break;
    case OMEGA:o=v2;break;
    default :  break;
    }
  switch(t1)
    {
    case PHI:  p=v1;break;
    case TAU:  t=v1;break;
    case OMEGA:o=v1;break;
    default :  break;
    }
  
  p0 = p;
  t0 = t;
  o0 = o;
  dt = timestep;
  
  pF = tF = oF = 0; //replace with compute forces?
  e = 0;            //replace with compute energy?
  track = 0;
  
  mob_p =  mob_t =  mob_o = 1;
  
}


//real functions:

double Bead::getP(){return p;}
double Bead::getT(){return t;}
double Bead::getO(){return o;}
double Bead::getdt(){return dt;}

void Bead::setP(double p_in){p = p_in;}
void Bead::setT(double t_in){t = t_in;}
void Bead::setO(double o_in){o = o_in;}
void Bead::setMobP(double mob_in){mob_p = mob_in;}
void Bead::setMobT(double mob_in){mob_t = mob_in;}
void Bead::setMobO(double mob_in){mob_o = mob_in;}
void Bead::setdt(double timestep){dt = timestep;}


int Bead::changeTrackSet(double T, int R,
                         double J, double Q, double Q2){
  if (track){
    track = 0;
    cout << "Tracking disabled\n";
    return 0;
  }
  else{
    track = 1;
    cout << "Tracking enabled\n";
    filename.setf(ios::fixed, ios::floatfield);
    filename.precision(4);
    filename.str("");
    filename << "bead_tracker_3d_T_"<<T<<"_R_"<<R<<"_Q_"<<Q<<".txt";
    
    pFile = fopen(filename.str().c_str(),"a");
    fprintf(pFile, "New Tracking: \n J=%5.4f \n Q=%5.4f \n Q2=%5.4f \n", J, Q, Q2);
    fclose(pFile);
    doTrack(T,R,J,Q,Q2);
    return 1;
  }
}


Bead::Phase Bead::decide_phase(){
  //First check for errors (tau < -.2, omega < -.2, etc.)


  //tolerances could have variable size, but for now they are constant
  if (p < .05 && p > -.05)
    if (t < .05)
      if (o < .05)
        return FLUID;;;
  
  if (p > .05 || p < -.05)
    if (t < .05 && t > -.2)
      if (o < .05 && o > -.2)
        return PHASE_SEP;;;

  if (p > .05){
    if (t > .05){

      if (o > 2*t - 1){
        if (t > o + .05)
          return SUPER_FERRO;
        if (t < o + .05 && t > o - .1)
          return FERRO;
      }

      else return BAD_FERRO;

    }

    if (o > .2){

      if (o > 2*t - 1){
        if (t < .1 && o > t + .2)
          return SUPER_NEMATIC;
        if (o > t + .2)
          return NEMATIC;
      }

      else return BAD_FERRO;
        
    }
  }      
        
  return NO;

}

void Bead::cout_phase(Phase phz){

  switch (phz)
    {
    case FLUID:
      cout << "Fluid phase, phi is near zero\n";
      break;
    case PHASE_SEP:
      cout << "Solid/Gas separated phase, phi is far from zero, but specific order parameters are near zero.\n";

      break;
    case FERRO:
      cout << "Ferro phase, tau is reasonably sized and near omega.\n";
      break;
    case SUPER_FERRO:
      cout << "SUPER Ferro phase, tau is reasonably sized and significantly larger than omega. (Note: Tau cannot be near 1!)\n";
      break;
    case NEMATIC:
      cout << "Nematic phase, omega is reasonably sized, and notably larger than tau.\n";
      break;
    case SUPER_NEMATIC:
      cout << "SUPER Nematic phase, omega is reasonably sized, and tau is very near to zero.\n";
      break;
    case BAD_FERRO:
      cout << "BAD FERRO (non-phase). Constraint on tau vs. omega not obeyed!\n";
      break;
    default:
      cout << "Derp, phase not defined!";
      break;
    }
  cout << "Phi: "<<p<<"\tTau: "<<t<<"\tOmega: "<<o<<"\n";
  

}


void Bead::heun_step(double T, int R,
                     double J, double Q, double Q2){
  //computes the force at two positions, and puts the bead in some sort of
  //force-averaged position
  
  // at the original position
        
  compute_forces_mft(T,R,J,Q,Q2);
  
  double tmp_pF = pF;
  double tmp_tF = tF;
  double tmp_oF = oF;
  
  p0 = p;
  t0 = t;
  o0 = o;
    
  p += dt * pF;
  t += dt * tF;
  o += dt * oF;
  
  //cout << pF; debug

  check_bounds();

  //at the shifted position
  compute_forces_mft(T,R,J,Q,Q2);
  
  tmp_pF += pF;
  tmp_tF += tF;
  tmp_oF += oF;
  
  p = p0 + 0.5 * dt * tmp_pF;
  t = t0 + 0.5 * dt * tmp_tF;
  o = o0 + 0.5 * dt * tmp_oF;

  check_bounds();
  
  doTrack(T, R, J, Q, Q2);
  
}

void Bead::check_bounds(){

  //Also consider the nematic boundary, that w cannot be less than 2t - 1.
  //This comes from the fact that any ferromagnetic alignment is also nematic!

  /*
  if (o < (2*t - 1)){
    //we want to project o and t to the closest allowed point; this is analytically solved
    double p1o = 0;
    double p1t = .5;
    double p2o = 1;
    double p2t = 1;

    //Parameter which interpolates between p1 and p2, between 0 and 1
    // (length of p1 -> p2 is sqrt[1.25])
    double u = ((o - p1o)*(p2o-p1o) + (t - p1t)*(p2t - p1t)) 
                        / (1.25);
    
    o = p1o + u * (p2o - p1o);
    t = p1t + u * (p2t - p1t);

    //cout << "Omega/Tau out of range!";
  }
  */

  //if (p < PMIN) p = PMIN;
  //if (t < TMIN) t = TMIN;
  //if (o < OMIN) o = OMIN;
  //if (p > PMAX) p = PMAX;
  //if (t > TMAX) t = TMAX;
  //if (o > OMAX) o = OMAX;

  //double rho = (p + 1)/2.0; //This is the expectation value of phi
  //if (o > rho) o = rho;
  //if (t > rho) t = rho;


}


void Bead::compute_forces_mft(double T, int R,
                              double J, double Q, double Q2){
  //The analytic function being used goes in here
  //**Assumes that z = 4
  double z = 4;
  double beta = 1.0/T;
  double rho = (p + 1.0) / 2.0;

  //The "chemical potential" term in the exponent is u = -Jz/2 - T ln(R)
  double expon = exp(beta*(J*rho*z - J*z/2));


  if (R==0) exit(1);
  double denom = 1.0 + ( (1.0 / R) * 
                         expon * 
                         (2.0 * exp(beta*o*Q2*z)*cosh(beta*t*Q*z) + 2.0*exp(-beta*o*Q2*z) + R - 4)
                         );

  //  cout << denom << "\n"; //debug
  
  //Compute the partial derivatives of energy in each direction
  pF = ( J*rho*z - 
         (J*z*(denom - 1)) / denom)   * mob_p * 2.0; // factor of 2 for jacobian

  tF = ( Q* t *z - 
         (2.0*Q *z*expon*exp(sinh(beta*Q*t*z))  / R / denom)) * mob_t; 

  oF = (Q2* o *z - 
        (2.0*Q2 *z*expon* (-exp(-beta*Q2*o*z)
                           +exp( beta*Q2*o*z)*cosh(beta*Q*t*z) ))  / R / denom) * mob_o;
  
  //Force is the negative gradient
  pF = -pF;
  tF = -tF;
  oF = -oF;
  
  e = (1.0/2.0)*z*(J*rho*rho + Q*t*t + Q2*o*o)-log(denom)*T;
  
}



int Bead::doTrack(double T, int R, double J, double Q, double Q2){
  if (track){
    pFile = fopen(filename.str().c_str(),"a");
    fprintf(pFile, "%5.4f %5.4f %5.4f %5.4f\n", p, t, o, e);
    fclose(pFile);
    return 1;
  }
  else{return 0;}
  
}







