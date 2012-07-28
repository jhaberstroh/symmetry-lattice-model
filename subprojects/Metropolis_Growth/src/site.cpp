//Site.cpp
#include "site.h"



Site::Site(int R, double occ_probability, int rot_preference, double rot_probability, MTRand* rng)
  : m_R(R){
  //When no random number generator is given, the site is created occupied and in the specified preferred
  // rotational direction.
  if (R >= 1){
    
    if (R % 4 == 0){
      //TODO (jhaberstroh@lbl.gov) Include input for different symmetry numbers
      //                           to make this condition apply for higher symmetry
      //                           numbers than 4 (aka nematic)
      //Ensures that R is a multiple of four.
      rot_preference = ((rot_preference %R) +R) %R;
      if (rng != 0){
        if (rng->rand() < rot_probability && rot_preference < R)
          m_rot = rot_preference;
        else
          m_rot = rng->randInt(R-1); //random integer from [0,R-1]
      }
      else
        m_rot = rot_preference;
    }
    else{
      //If R is not a multiple of four,
      // set R to be the closest larger multiple of four.
      R = (R / 4 + 1) * 4;
      rot_preference = ((rot_preference %R) +R) %R;
      if (rng != 0){
        if (rng->rand() < rot_probability && rot_preference < R)
          m_rot = rot_preference;
        else
          m_rot = rng->randInt(R-1); //random integer from [0,R-1]
      }
      else{
        m_rot = rot_preference;
      }
      cout << "R was not given as a multiple of four, correcting by rounding up\n"
           << "to the nearest multiple of four.\n";
    }
  }
  else{
    cout << "Trying to initialize a site with a negative number of rotational states; "
         << R << " \n"
         << "instead, initializing R to 4.\n";
      R = 4;
      rot_preference = ((rot_preference %R) +R) %R;
      if (rng != 0){
        if (rng->rand() < rot_probability && rot_preference < R)
          m_rot = rot_preference;
        else
          m_rot = rng->randInt(R-1); //random integer from [0,R-1]
      }
      else
        m_rot = rot_preference;

      m_occ = 0;
    }

  if (rng != 0){
    if (rng->rand() < occ_probability)
      m_occ = 1;
    else
      m_occ = 0;
  }
  else{
    m_occ = 1;
  }
}


int TestSiteCode(){
  MTRand rng;

  Site s1(8, .5, 0, 0, &rng);
  cout << "Site occupancy: "  <<s1.occ()<<endl;
  cout << "Rotational state: "<<s1.rot()<<endl;
  
  for (int i = 0; i < 25 ; i++){
    if (s1.occ()){
      if (s1.AttemptRot(1, .5, rng)){
        cout << "Rotate succeeded and ";
        if (s1.AttemptOcc(.5, rng)) cout <<"de-occupy succeeded!"<<endl;
        else cout << "de-occupy failed."<<endl;
      }
      else  cout << "Rotate failed."<<endl;
    }
    else{
      s1.RandRot(rng);
      if (s1.AttemptOcc(.5, rng)){
        cout << "Re-occupy succeeded!"<<endl;
      }
      else {
        cout << "Re-occupy failed."<<endl;
      }
    }
    cout << "End of iteration "<< i << "." << endl;
    cout << "Site occupancy: "  <<s1.occ()<<endl;
    cout << "Rotational state: "<<s1.rot()<<endl;
  }

  return 0;

}




/*--------------------------------------------------
4  RETIRED CODE, possibly useful for porting to new code


double Site::attemptOcc(Site::svec neighbors, double pdel, double T, pvec params, orderparam* order){
  if (!occ)
    setRot(randRot());

  int dN1bond = 0;
  int dN2bond = 0;
  double dE = occDE(neighbors, params, &dN1bond, &dN2bond);
  dE += chemPotential(T, params) * ((occ * 2.0) - 1.0); 
  //NOTE :: chemical potential has a different sign depending on the occupation  

  double temp_prob = 0;
  if (occ){
    temp_prob = (1/(pdel * R)) * exp(-dE/T);
  }
  if (!occ){
    temp_prob = pdel * R * exp(-dE/T);
  }


  if (rng->rand() < temp_prob){
    changeOcc();

    vector<int> opts;
    opts.push_back((occ*2)-1);
    updateOP(orderparam::OCC, opts);

    opts.clear();
    opts.push_back((occ*2)-1); opts.push_back(rot); opts.push_back(0); opts.push_back(dN1bond);
    updateOP(orderparam::N1, opts);

    opts.clear();
    opts.push_back((occ*2)-1); opts.push_back(rot); opts.push_back(0); opts.push_back(dN2bond);
    updateOP(orderparam::N2, opts);
    
    return dE;
  }
  return 0; //if all attempts fail, there is no energy change to report
}


double Site::attemptRot(Site::svec neighbors, double T, pvec params, orderparam* order){
  if (occ){
    int plus_minus = ((rng->rand() < .5) * 2) - 1;
    double dE = 0;
    int dN1bond = 0;
    int dN2bond = 0;
    dE = rotDE(neighbors, ((rot + plus_minus)+R) % R, params, &dE, &dN1bond, &dN2bond);
    
    if (rng->rand() < exp(-dE/T)){
      moveRot(plus_minus);

      vector<int> opts;
      opts.push_back(0); opts.push_back(rot); opts.push_back(plus_minus); opts.push_back(dN1bond);
      order->updateOP(orderparam::N1, opts);

      opts.clear();
      opts.push_back(0); opts.push_back(rot); opts.push_back(plus_minus); opts.push_back(dN2bond);
      order->updateOP(orderparam::N2, opts);

      return dE;
    }
  }
  
  return 0;  
}

*/





