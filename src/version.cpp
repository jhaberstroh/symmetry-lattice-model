//version.cpp
#include "version.h"

int main(){
  Version v;
  cout << "Version updated from "<< v.GetCurrentVersion();
  v.UpdateVersion();
  cout << " to " << v.GetCurrentVersion() << endl;
}
