//logfile.h
#ifndef __LOGFILE_H_INCLUDED__
#define __LOGFILE_H_INCLUDED__


#include <iostream>
#include <sstream>
#include <string>

#include "filehandler.h"

/*
A LogFile is to be owned by a MonteCarlo object, and passed as a pointer member
variable to the LatticeFile and the OrderParamFile.
Read methods for the logs wihch are generated can be implemented in this class for use in the GUI.
*/

class LogFile : public FileHandler{
    //m_write_buffer is the write buffer member variable inherited from FileHandler
    public:
        LogFile();

    //----------------------------------------------------
    //Dummy code:
        void UpdateLog(std::string file_name);
        std::string ReadLog();
        void InsertHeader(){} //virtual function does nothing

    private:
        std::ostringstream m_last_write_file;
        //The "unique" code for accessing the type of log (lattice, orderparam, etc.)
        std::string m_access_code;
};

#endif // __LOGFILE_H_INCLUDED__
