#include "../include/system.h"

#include <algorithm>
#include <iostream>

#include "../include/linux_parser.h"
#include "../include/process.h"
#include "../include/processor.h"

Processor& System::Cpu() { return cpu_; }

vector<Process>& System::Processes() {
  vector<int> pids = LinuxParser::Pids();

  for (int pid : pids) {
    Process process(pid);

    processes_.push_back(process);
  }

  std::sort(processes_.rbegin(), processes_.rend());

  return processes_;
}

std::string System::Kernel() { return LinuxParser::Kernel(); }

float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }

int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

long int System::UpTime() { return LinuxParser::UpTime(); }