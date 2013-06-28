#include "logfile.h"
using namespace std;

LogFile::LogFile():
    FileHandler("./.log")
{
    m_last_write_file.str("");
    //^^
    //TODO: Use an ios mode which makes more sense than "append"
    //
}

void LogFile::UpdateLog(string file_name)
{
    m_last_write_file.str(file_name);
}

string LogFile::ReadLog()
{
    return m_last_write_file.str();
}
