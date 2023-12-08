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
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
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

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  string line;
  float totalMemory = 0.0;
  float freeMemory = 0.0;
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);

  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      string key;
      float value;

      linestream >> key >> value;

      if (key == "MemTotal:") {
        totalMemory = value;
      } else if (key == "MemFree:") {
        freeMemory = value;
        break; // No need to continue reading the file
      }
    }
  }

  // Calculate memory utilization as a percentage
  return (totalMemory - freeMemory) / totalMemory;
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() {
  long uptime = 0;
  string line;
  std::ifstream filestream(kProcDirectory + kUptimeFilename);

  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> uptime;
  }

  return uptime;
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() {
  string line;
  long totalJiffies = 0;
  std::ifstream filestream(kProcDirectory + kStatFilename);

  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      string key;
      long value;

      linestream >> key;

      if (key == "cpu") {
        // Read CPU usage values from /proc/stat for all CPUs
        linestream >> value; // user
        linestream >> value; // nice
        linestream >> value; // system
        linestream >> value; // idle
        linestream >> value; // iowait
        linestream >> value; // irq
        linestream >> value; // softirq
        linestream >> value; // steal
        linestream >> value; // guest
        linestream >> value; // guest_nice

        // Calculate the total jiffies for all CPUs
        totalJiffies = value;
        break; // No need to continue reading the file
      }
    }
  }

  return totalJiffies;
}

// TODO: Read and return the number of active jiffies for a PID
long LinuxParser::ActiveJiffies(int pid) {
  long activeJiffies = 0;
  string line;
  string token;
  vector<string> stat_values;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);

  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);

    // Tokenize the line
    while (linestream >> token) {
      stat_values.push_back(token);
    }

    // Extract values for user, system, and other CPU usage
    if (stat_values.size() >= 17) {
      long utime = std::stol(stat_values[13]);
      long stime = std::stol(stat_values[14]);
      long cutime = std::stol(stat_values[15]);
      long cstime = std::stol(stat_values[16]);

      // Calculate active jiffies for the process
      activeJiffies = utime + stime + cutime + cstime;
    }
  }

  return activeJiffies;
}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
  long activeJiffies = 0;
  string line;
  std::ifstream filestream(kProcDirectory + kStatFilename);

  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      string key;
      long value;

      linestream >> key;

      if (key == "cpu") {
        // Read CPU usage values from /proc/stat for all CPUs
        linestream >> value; // user
        // linestream >> value; // nice
        // linestream >> value; // system
        // linestream >> value; // idle
        // linestream >> value; // iowait
        // linestream >> value; // irq
        // linestream >> value; // softirq
        // linestream >> value; // steal
        // linestream >> value; // guest
        // linestream >> value; // guest_nice

        // Calculate active jiffies for all CPUs
        while (linestream >> value) {
          activeJiffies += value;  // Sum all CPU usage values for the specified CPU
        }
        break; // No need to continue reading the file
      }
    }
  }

  return activeJiffies;
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
  long idleJiffies = 0;
  string line;
  std::ifstream filestream(kProcDirectory + kStatFilename);

  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      string key;
      long value;

      linestream >> key;

      if (key == "cpu") {
        // Read CPU usage values from /proc/stat for all CPUs
        linestream >> value; // user
        // linestream >> value; // nice
        // linestream >> value; // system
        // linestream >> value; // idle (this is what we want)
        // linestream >> value; // iowait

        // Calculate idle jiffies for all CPUs
        while (linestream >> value) {
          idleJiffies += value;  // Sum all CPU usage values for the specified CPU
        }
        // break; // No need to continue reading the file
        return idleJiffies;
      }
    }
  }

  
}

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  vector<string> cpuUtilization;
  string line;
  std::ifstream filestream(kProcDirectory + kStatFilename);

  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      string key;

      linestream >> key;

      if (key == "cpu") {
        // Read the entire line and store it as CPU utilization
        cpuUtilization.push_back(line);
        break; // No need to continue reading the file
      }
    }
  }

  return cpuUtilization;
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  int totalProcesses = 0;
  string line;
  std::ifstream filestream(kProcDirectory + kStatFilename);

  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      string key;

      linestream >> key;

      if (key == "processes") {
        linestream >> totalProcesses;
        break; // No need to continue reading the file
      }
    }
  }

  return totalProcesses;
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  int runningProcesses = 0;
  string line;
  std::ifstream filestream(kProcDirectory + kStatFilename);

  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      string key;

      linestream >> key;

      if (key == "procs_running") {
        linestream >> runningProcesses;
        break; // No need to continue reading the file
      }
    }
  }

  return runningProcesses;
}

// TODO: Read and return the command associated with a process
string LinuxParser::Command(int pid) {
  string command;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kCmdlineFilename);

  if (filestream.is_open()) {
    std::getline(filestream, command);
  }

  return command;
}

// TODO: Read and return the memory used by a process
string LinuxParser::Ram(int pid) {
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);

  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;

      if (key == "VmSize:") {
        // Convert kB to MB for consistency
        long ram_kb = stol(value);
        long ram_mb = ram_kb / 1024;
        return to_string(ram_mb);
      }
    }
  }

  return string(); // Return an empty string if not found
}

// TODO: Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) {
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);

  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;

      if (key == "Uid:") {
        return value;
      }
    }
  }

  return string(); // Return an empty string if not found
}

// TODO: Read and return the user associated with a process
string LinuxParser::User(int pid) {
  string uid = Uid(pid);
  string line;
  string username;
  string x;
  string user_id;

  std::ifstream filestream(kPasswordPath);

  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> username >> x >> user_id;

      if (user_id == uid) {
        return username;
      }
    }
  }

  return string(); // Return an empty string if not found
}

// TODO: Read and return the uptime of a process
long LinuxParser::UpTime(int pid) {
  string line;
  long uptime = 0;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);

  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);

    // Skip several fields to get to the 'starttime' field (22nd field)
    for (int i = 0; i < 21; i++) {
      linestream.ignore(std::numeric_limits<std::streamsize>::max(), ' ');
    }

    long starttime;
    linestream >> starttime;
    uptime = UpTime() - (starttime / sysconf(_SC_CLK_TCK)); // Convert clock ticks to seconds
  }

  return uptime;
}
