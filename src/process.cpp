#include "../include/process.h"

#include <unistd.h>

#include <cctype>
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

  return utilization;
}

// TODO: Return the command that generated this process
string Process::Command() { return string(); }

// TODO: Return this process's memory utilization
string Process::Ram() { return string(); }

string Process::User() { return LinuxParser::User(pid_); }

long int Process::UpTime() {
  return LinuxParser::UpTime() -
         (LinuxParser::UpTime(pid_) / sysconf(_SC_CLK_TCK));
}

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a [[maybe_unused]]) const {
  return true;
}