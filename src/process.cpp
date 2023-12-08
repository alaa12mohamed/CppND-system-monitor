#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include "linux_parser.h"
#include "process.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid) : pid_(pid) {
  cpuUtilization_ = CpuUtilization();
  command_ = Command();
  ram_ = Ram();
  user_ = User();
  uptime_ = UpTime();
}

int Process::Pid() const {
  return pid_;
}

float Process::CpuUtilization() const {
  long activeJiffies = LinuxParser::ActiveJiffies(pid_);
  long systemUptime = LinuxParser::UpTime();
  long processUptime = LinuxParser::UpTime(pid_);
  
  if (systemUptime <= 0 || processUptime <= 0) {
    return 0.0; // Return 0 if unable to calculate.
  }

  return static_cast<float>(activeJiffies) / sysconf(_SC_CLK_TCK) / processUptime;
}

std::string Process::Command() const {
  return LinuxParser::Command(pid_);
}

std::string Process::Ram() const {
  return LinuxParser::Ram(pid_);
}

std::string Process::User() const {
  return LinuxParser::User(pid_);
}

long int Process::UpTime() const {
  return LinuxParser::UpTime(pid_);
}

bool Process::operator<(Process const& a) const {
  return cpuUtilization_ > a.cpuUtilization_;
}
