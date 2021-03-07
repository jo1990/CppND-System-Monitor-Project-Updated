#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include <unistd.h>


#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid) 
{
    _PID = pid;
    CalcCpuUtil();
    _Command = LinuxParser::Command(_PID);
    _Ram = LinuxParser::Ram(_PID);
    _User = LinuxParser::User(_PID);
    _UpTime = LinuxParser::UpTime(_PID);

}

// Return this process's ID
int Process::Pid() { return _PID; }

// Return this process's CPU utilization
float Process::CpuUtilization() 
{
    return _cpuUtilization;
}

// Return the command that generated this process
string Process::Command() { return _Command; }

// Return this process's memory utilization
string Process::Ram() { return _Ram; }

// Return the user (name) that generated this process
string Process::User() { return _User; }

// Return the age of this process (in seconds)
long int Process::UpTime() 
{
    return _UpTime; 
}

// Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const& a) const 
{ 
    if(this->_cpuUtilization > a._cpuUtilization)
    {
        return true;
    }
    else
    {
        return false;
    } 
}

void Process::CalcCpuUtil()
{
        string line;
    string key;
    string value;

    long utime = 0;
    long stime = 0;
    long cutime = 0;
    long cstime = 0;
    long starttime = 0;
    long upTime;

    float total_time = 0;
    double seconds = 0;
    float cpu_usage=0;
    std::ifstream filestream(LinuxParser::kProcDirectory+ to_string(_PID)+LinuxParser::kStatFilename);
    if (filestream.is_open()) {
        while (std::getline(filestream, line)) 
        {
            std::replace(line.begin(), line.end(), ':', ' ');
            std::istringstream linestream(line);
            int index = 0;
            while (linestream  >> value) 
            {
                if(index == 13)
                {
                    utime = atol(value.c_str());
                    index ++ ;
                }
                else if(index == 14)
                {
                    stime = atol(value.c_str());;
                    index ++ ;
                }
                else if(index == 15)
                {
                    cutime = atol(value.c_str());
                    index ++ ;
                }
                else if(index == 16)
                {
                    cstime = atol(value.c_str());
                    index ++ ;
                }
                else if(index == 21)
                {
                    starttime = atol(value.c_str());
                    index ++ ;
                }
                else
                {
                    index ++ ;
                }
                
            }
        }
    }

    upTime = LinuxParser::UpTime() ;
    total_time = utime + stime +  cutime + cstime;
    seconds = upTime - (starttime/sysconf(_SC_CLK_TCK)) ;

    cpu_usage = (total_time/sysconf(_SC_CLK_TCK)/seconds);
    _cpuUtilization = cpu_usage;
}