#ifndef PROCESS_H
#define PROCESS_H

#include <string>

class Process {
 public:
  int Pid();
  std::string User();
  std::string Command();
  float CpuUtilization();
  std::string Ram();
  long int UpTime();
  bool operator<(Process const &a) const;
  Process(int pid): pid_(pid) {};

 private:
  int pid_;
};

#endif