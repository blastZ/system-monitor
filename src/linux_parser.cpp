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

vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());

  struct dirent* file;

  while ((file = readdir(directory)) != nullptr) {
    if (file->d_type == DT_DIR) {
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
  int total = 1;
  int free = 1;

  std::ifstream stream(kProcDirectory + kMeminfoFilename);

  string total_line;
  string free_line;
  string total_str, free_str;

  if (stream.is_open()) {
    std::getline(stream, total_line);
    std::getline(stream, free_line);

    std::istringstream total_linestream(total_line);
    std::istringstream free_linestream(free_line);

    total_linestream >> total_str >> total_str;
    free_linestream >> free_str >> free_str;

    total = std::stoi(total_str);
    free = std::stoi(free_str);
  }

  return (total - free) * 1.0f / total;
}

long LinuxParser::UpTime() {
  string uptime = "0";

  std::ifstream stream(kProcDirectory + kUptimeFilename);

  string line;

  if (stream.is_open()) {
    std::getline(stream, line);

    std::istringstream linestream(line);

    linestream >> uptime;
  }

  return std::stol(uptime);
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
  string total = "0";

  std::ifstream stream(kProcDirectory + kStatFilename);

  string line;
  string key, value;

  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);

      linestream >> key >> value;

      if (key == "processes") {
        total = value;
        break;
      }
    }
  }

  return std::stoi(total);
}

int LinuxParser::RunningProcesses() {
  string running = "0";

  std::ifstream stream(kProcDirectory + kStatFilename);

  string line;
  string key, value;

  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);

      linestream >> key >> value;

      if (key == "procs_running") {
        running = value;
        break;
      }
    }
  }

  return std::stoi(running);
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

string LinuxParser::User(int pid) {
  string user = "";

  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatusFilename);

  string line;
  string key, value;

  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);

      linestream >> key >> value;

      if (key == "Uid:") {
        user = value;
        break;
      }
    }
  }

  return user;
}

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