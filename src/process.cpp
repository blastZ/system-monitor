#include "../include/process.h"

#include <unistd.h>

#include <cctype>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "../include/linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

int Process::Pid() { return pid_; }

float Process::CpuUtilization() {
  float utilization = 0.0f;

  long total_time = LinuxParser::ActiveJiffies(pid_);
  long seconds = UpTime();

  utilization = (total_time * 1.0f / sysconf(_SC_CLK_TCK) / seconds);

  cpu_utilization_ = utilization;

  return utilization;
}

// TODO: Return the command that generated this process
string Process::Command() { return string(); }

string Process::Ram() { return LinuxParser::Ram(pid_); }

string Process::User() { return LinuxParser::User(pid_); }

long int Process::UpTime() {
  return LinuxParser::UpTime() -
         (LinuxParser::UpTime(pid_) / sysconf(_SC_CLK_TCK));
}

bool Process::operator<(Process const& a) const {
  return cpu_utilization_ < a.cpu_utilization_;
}