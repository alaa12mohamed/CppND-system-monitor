#ifndef PROCESS_H
#define PROCESS_H

#include <string>

class Process {
 public:
  Process(int pid);
  int Pid() const;
  float CpuUtilization() const;
  std::string Command() const;
  std::string Ram() const;
  std::string User() const;
  long int UpTime() const;
  bool operator<(Process const& a) const;

 private:
  int pid_;
  float cpuUtilization_;
  std::string command_;
  std::string ram_;
  std::string user_;
  long int uptime_;
};

#endif
