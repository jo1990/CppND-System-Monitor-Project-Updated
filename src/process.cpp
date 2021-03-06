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
}

// Return this process's ID
int Process::Pid() { return _PID; }

// Return this process's CPU utilization
float Process::CpuUtilization() 
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
    return _cpuUtilization;   
}

// Return the command that generated this process
string Process::Command() { return LinuxParser::Command(_PID); }

// Return this process's memory utilization
string Process::Ram() { return LinuxParser::Ram(_PID); }

// Return the user (name) that generated this process
string Process::User() { return LinuxParser::User(_PID); }

// Return the age of this process (in seconds)
long int Process::UpTime() 
{
    long clockTicks = LinuxParser::UpTime(_PID); 
    return clockTicks/sysconf(_SC_CLK_TCK); 
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