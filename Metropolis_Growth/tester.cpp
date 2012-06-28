//tester.cpp

#include "site.h"
#include "lattice.h"
#include "interaction.h"
#include "monteCarlo.h"


int main(){
  //in site.cpp
  TestSiteCode();

  cout << "\n\n\n\n Site Test finished. Press <Enter> to continue ot the Lattice Test." << endl;
  cin.get();
  cout << "\n\n\n\n";

  //in lattice.cpp
  TestLatticeCode();

  cout << "\n\n\n\n Lattice Test finished. Press <Enter> to continue ot the Lattice Test." << endl;
  cin.get();
  cout << "\n\n\n\n";

  //in interaction.cpp
  TestInteractionCode();

  cout << "\n\n\n\n Lattice Test finished. Press <Enter> to continue ot the Lattice Test." << endl;
  cin.get();
  cout << "\n\n\n\n";


  try{
    TestMonteCarloCode();
  }
  catch(vector_size_error& e){
    cout << "Vector size error on " << e.what() <<endl;
    cout << "Requested size: "<<e.m_requested_size << endl;
    cout << "Received size: "<<e.m_received_size << endl;
  }

  cout << "\n\n\n\n";
  
}
