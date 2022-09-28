#include "../include/linux_parser.h"

#include <dirent.h>
#include <unistd.h>

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using std::stof;
using std::string;
using std::to_string;
using std::vector;

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

string LinuxParser::Kernel() {
  string os, kernel, version;
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

float LinuxParser::MemoryUtilization() {
  vector<string> meminfo_list =
      GetLineListFromFile(kProcDirectory + kMeminfoFilename);

  vector<string> memtotal_list = GetLineListFromString(meminfo_list[0], ' ');
  int memtotal = std::stoi(memtotal_list.rbegin()[1]);

  vector<string> memfree_list = GetLineListFromString(meminfo_list[1], ' ');
  int memfree = std::stoi(memfree_list.rbegin()[1]);

  return (memtotal - memfree) * 1.0f / memtotal;
}

long LinuxParser::UpTime() {
  vector<string> uptime_list = GetLineListFromFile("/proc/uptime");

  vector<string> uptime_item_list = GetLineListFromString(uptime_list[0], ' ');

  return std::stol(uptime_item_list[0]);
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid [[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { return {}; }

int LinuxParser::TotalProcesses() {
  vector<string> line_list = GetLineListFromFile("/proc/stat");

  string processes_str = line_list.rbegin()[4];

  vector<string> str_list = GetLineListFromString(processes_str, ' ');

  return std::stoi(str_list[1]);
}

int LinuxParser::RunningProcesses() {
  vector<string> mem_list = GetLineListFromFile("/proc/stat");

  vector<string> mem_item_list =
      GetLineListFromString(mem_list.rbegin()[3], ' ');

  return std::stoi(mem_item_list[1]);
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid [[maybe_unused]]) { return string(); }

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid [[maybe_unused]]) { return string(); }

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid [[maybe_unused]]) { return string(); }

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid [[maybe_unused]]) { return string(); }

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid [[maybe_unused]]) { return 0; }

vector<string> LinuxParser::GetLineListFromStream(std::istream& stream,
                                                  char delimiter) {
  vector<string> line_list{};

  while (stream.good()) {
    string line;

    std::getline(stream, line, delimiter);

    line_list.push_back(line);
  }

  return line_list;
}

vector<string> LinuxParser::GetLineListFromString(string str, char delimiter) {
  std::stringstream str_stream(str);

  return GetLineListFromStream(str_stream, delimiter);
}

vector<string> LinuxParser::GetLineListFromFile(string file_path,
                                                char delimiter) {
  std::ifstream file_stream;

  file_stream.open(file_path);

  vector<string> line_list{};

  if (file_stream.is_open()) {
    line_list = GetLineListFromStream(file_stream, delimiter);
  }

  return line_list;
}