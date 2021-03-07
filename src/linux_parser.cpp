#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel;
  string line;
  string version;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> kernel >> version;
  }
  return version;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// Read and return the system memory utilization
float LinuxParser::MemoryUtilization() 
{ 
  long memTotal;
  long memFree; 
  float memUtil;

  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory+kMeminfoFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "MemTotal") {
          memTotal = stol(value);
        }
        else if(key == "MemFree")
        {
          memFree = stol(value);
        }
      }
    }
  }
  memUtil=(memTotal-memFree)/(float)memTotal;
  return memUtil; 
}

// Read and return the system uptime
long LinuxParser::UpTime() 
{
  string line;
  string value;
  std::ifstream filestream(kProcDirectory+ kUptimeFilename);
  if (filestream.is_open()) 
  {
      getline(filestream, line);
      std::istringstream linestream(line);
      while (linestream >> value) 
      {
          return std::stol(value);
      }
  }
  return 0; 
}

// Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// Read and return the number of active jiffies for a PID
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() 
{ 
  string cpu_user;
  string cpu_nice;
  string cpu_system;
  string cpu_idle;
  string cpu_iowait;
  string cpu_irq;
  string cpu_softirq;
  string cpu_steal;
  string cpu_guest;
  string cpu_guest_nice;

  string line;
  string key;

  vector<string> cpu_data;

  std::ifstream filestream(kProcDirectory+ kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
      std::istringstream linestream(line);
      linestream >> key >> cpu_user >> cpu_nice >> cpu_system >> cpu_idle >> cpu_iowait >> cpu_irq >> cpu_softirq 
                                                                          >> cpu_steal >> cpu_guest >> cpu_guest_nice;
      
        if (key == "cpu") 
        {
          cpu_data.push_back(cpu_guest_nice);
          cpu_data.push_back(cpu_guest);
          cpu_data.push_back(cpu_steal);
          cpu_data.push_back(cpu_softirq);
          cpu_data.push_back(cpu_irq);
          cpu_data.push_back(cpu_iowait);
          cpu_data.push_back(cpu_idle);
          cpu_data.push_back(cpu_system);
          cpu_data.push_back(cpu_nice);
          cpu_data.push_back(cpu_user);
        }
  }
  return cpu_data; 
}

// Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory+ kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) 
    {
      std::istringstream linestream(line);
      while (linestream >> key >> value) 
      {
        if (key == "processes") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return std::stoi(value);
        }
      }
    }
  }
  return 0; 
  }

// Read and return the number of running processes
int LinuxParser::RunningProcesses() 
{  
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory+ kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) 
    {
      std::istringstream linestream(line);
      while (linestream >> key >> value) 
      {
        if (key == "procs_running") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return std::stoi(value);
        }
      }
    }
  }
  return 0; 
}

// Read and return the command associated with a process
string LinuxParser::Command(int pid) 
{ 
  string line;
  string cmdLine = "";

  std::ifstream filestream(kProcDirectory+ to_string(pid)+kCmdlineFilename);
  if (filestream.is_open()) 
  {
    std::getline(filestream, line);
    cmdLine = line;
    return cmdLine;
  }
      
  return cmdLine; 
}

// Read and return the memory used by a process
string LinuxParser::Ram(int pid) 
{ 
  string line;
  string key;
  string value;
  int KB_To_MB = 0;
  std::ifstream filestream(kProcDirectory+ to_string(pid)+kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "VmSize") {
          KB_To_MB = atoi(value.c_str()) * 0.001;
          return to_string(KB_To_MB);
        }
      }
    }
  }
  return to_string(KB_To_MB);
}

// Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) 
{ 
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory+ to_string(pid)+kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "Uid") {
          return value;
        }
      }
    }
  }
  return value; 
}

// Read and return the user associated with a process
string LinuxParser::User(int pid) 
{ 
  string UID;
  string userName;

  UID = Uid(pid);

  string line;
  string key;
  string value;
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> userName >>key >>value) {
        if (value == UID) {
          return userName;
        }
      }
    }
  }

  return userName; 
}

// Read and return the uptime of a process
long LinuxParser::UpTime(int pid) 
{ 
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory+ to_string(pid)+kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      int count = 0;
      while (linestream  >> value) {
        if(count == 21)
        {
          return UpTime()- atol(value.c_str())/sysconf(_SC_CLK_TCK);
        }
        else
        {
          count++;
        }
        }
      }
    }

  return 0; 
}