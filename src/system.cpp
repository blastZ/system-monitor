#include "../include/system.h"

#include <unistd.h>

#include <cstddef>
#include <fstream>
#include <iostream>
#include <istream>
#include <set>
#include <sstream>
#include <string>
#include <vector>

#include "../include/format.h"
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

std::string System::Kernel() {
  string kernel_version = "";

  std::ifstream kernel_file;

  kernel_file.open("/proc/version");

  if (kernel_file.is_open()) {
    vector<string> segment_list;

    while (kernel_file.good()) {
      string kernel_string;

      std::getline(kernel_file, kernel_string, ' ');

      segment_list.push_back(kernel_string);
    }

    kernel_version = segment_list[2];
  }

  return kernel_version;
}

float System::MemoryUtilization() {
  int mem_total_number = 0;
  int mem_free_number = 0;

  std::ifstream mem_file;

  mem_file.open("/proc/meminfo");

  if (mem_file.is_open()) {
    if (mem_file.good()) {
      string mem_total_str;
      std::getline(mem_file, mem_total_str);

      vector<string> segment_list;

      std::stringstream mem_total_stream(mem_total_str);

      while (mem_total_stream.good()) {
        string segment;
        std::getline(mem_total_stream, segment, ' ');
        segment_list.push_back(segment);
      }

      mem_total_number = std::stoi(segment_list.rbegin()[1]);
    }

    if (mem_file.good()) {
      string mem_free_str;
      std::getline(mem_file, mem_free_str);

      vector<string> segment_list;

      std::stringstream mem_free_stream(mem_free_str);

      while (mem_free_stream.good()) {
        string segment;
        std::getline(mem_free_stream, segment, ' ');
        segment_list.push_back(segment);
      }

      mem_free_number = std::stoi(segment_list.rbegin()[1]);
    }
  }

  if (mem_total_number == 0) {
    return 0.0f;
  }

  return (mem_total_number - mem_free_number) * 1.0f / mem_total_number;
}

std::string System::OperatingSystem() {
  std::ifstream os_file;

  os_file.open("/etc/os-release");

  if (os_file.is_open()) {
    while (os_file.good()) {
      string os_line;
      std::getline(os_file, os_line);

      std::stringstream os_line_stream(os_line);
      vector<string> segment_list;

      while (os_line_stream.good()) {
        string segment;
        std::getline(os_line_stream, segment, '=');

        segment_list.push_back(segment);
      }

      if (segment_list[0] == "PRETTY_NAME") {
        return segment_list[1];
      }
    }
  }

  return string();
}

int System::RunningProcesses() {
  vector<string> mem_list = Format::GetLineListFromFile("/proc/stat");

  vector<string> mem_item_list =
      Format::GetLineListFromString(mem_list.rbegin()[3], ' ');

  return std::stoi(mem_item_list[1]);
}

int System::TotalProcesses() {
  vector<string> line_list = Format::GetLineListFromFile("/proc/stat");

  string processes_str = line_list.rbegin()[4];

  vector<string> str_list = Format::GetLineListFromString(processes_str, ' ');

  return std::stoi(str_list[1]);
}

long int System::UpTime() {
  vector<string> uptime_list = Format::GetLineListFromFile("/proc/uptime");

  vector<string> uptime_item_list =
      Format::GetLineListFromString(uptime_list[0], ' ');

  return std::stol(uptime_item_list[0]);
}