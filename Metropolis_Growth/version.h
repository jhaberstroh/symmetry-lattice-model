//version.h
/*--------------------------------------------------
  Version is a very simple class to track the current
  version number, and to mark possible incompatibilities
  between previously saved files and current files.
  --------------------------------------------------*/


#ifndef __FILEHANDLER_H_INCLUDED__
#define __FILEHANDLER_H_INCLUDED__

#include <iostream>
#include <fstream>
#include <sstream>
#include <iterator>
#include <vector>

using namespace std;

static const char*   version_file_name = "./.version";


struct tokens: std::ctype<char> 
{
 tokens(): std::ctype<char>(get_table()) {}
  
  static std::ctype_base::mask const* get_table()
  {
    typedef std::ctype<char> cctype;
    static const cctype::mask *const_rc= cctype::classic_table();
    
    static cctype::mask rc[cctype::table_size];
    std::memcpy(rc, const_rc, cctype::table_size * sizeof(cctype::mask));
    
    rc[','] = std::ctype_base::space; 
    rc[' '] = std::ctype_base::space; 
    return &rc[0];
  }
};



class Version{



 public: 
  enum UpdateSize{kSmallUpdate,kMediumUpdate,kLargeUpdate};
  ifstream version_file_in;
  ofstream version_file_out;
  int large_version;
  int medium_version;
  int small_version;

 Version(): version_file_in(version_file_name), version_file_out()  {
    string s;
    version_file_in >> s;
    stringstream ss(s);
    ss.imbue(std::locale(std::locale(), new tokens()));
    istream_iterator<string> begin(ss);
    istream_iterator<string> end;
    vector<string> vstrings(begin,end);
             
    large_version = atoi(vstrings[0].c_str());
    medium_version = atoi(vstrings[1].c_str());
    small_version = atoi(vstrings[2].c_str());
    version_file_in.close();
  }
  
  inline void UpdateVersion(UpdateSize s = kSmallUpdate){
    switch (s){
    case kSmallUpdate:
      small_version++; break;
    case kMediumUpdate:
      medium_version++; break;
    case kLargeUpdate:
      large_version++; break;
    }
    version_file_out.open(version_file_name);
    version_file_out << large_version <<"."<<medium_version << "." << small_version;
    version_file_out.close();
  }

  inline string GetCurrentVersion(){
    ostringstream function_output("");
    function_output << large_version <<"."<<medium_version << "." << small_version;
    return function_output.str();
  }


};


#endif //__FILEHANDLER_H_INCLUDED__
