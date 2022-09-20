#include "../include/system.h"

#include <unistd.h>

#include <cstddef>
#include <fstream>
#include <iostream>
#include <set>
#include <sstream>
#include <string>
#include <vector>

#include "../include/process.h"
#include "../include/processor.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

// TODO: Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes() { return processes_; }

// TODO: Return the system's kernel identifier (string)
std::string System::Kernel() { return string(); }

// TODO: Return the system's memory utilization
float System::MemoryUtilization() { return 0.0; }

std::string System::OperatingSystem() {
  std::ifstream os_file;

  os_file.open("/etc/os-release");

  if (os_file.is_open()) {
    while (os_file.good()) {
      string os_line;
      std::getline(os_file, os_line);

      std::stringstream os_line_stream(os_line);
      string segment;
      vector<string> segment_list;

      while (std::getline(os_line_stream, segment, '=')) {
        segment_list.push_back(segment);
      }

      if (segment_list[0] == "PRETTY_NAME") {
        return segment_list[1];
      }
    }
  }

  return string();
}

// TODO: Return the number of processes actively running on the system
int System::RunningProcesses() { return 0; }

// TODO: Return the total number of processes on the system
int System::TotalProcesses() { return 0; }

// TODO: Return the number of seconds since the system started running
long int System::UpTime() { return 0; }