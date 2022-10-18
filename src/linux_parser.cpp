#include "../include/linux_parser.h"

#include <dirent.h>
#include <unistd.h>

#include <fstream>
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

long LinuxParser::Jiffies() { return ActiveJiffies() + IdleJiffies(); }

long LinuxParser::ActiveJiffies(int pid) {
  long active = 0;

  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatFilename);

  vector<string> stats;

  if (stream.is_open()) {
    string line;

    while (std::getline(stream, line, ' ')) {
      stats.push_back(line);
    }

    active =
        stol(stats[13]) + stol(stats[14]) + stol(stats[15]) + stol(stats[16]);
  }

  return active;
}

long LinuxParser::ActiveJiffies() {
  vector<string> cpu_states = CpuUtilization();

  return std::stol(cpu_states[CPUStates::kUser_]) +
         std::stol(cpu_states[CPUStates::kNice_]) +
         std::stol(cpu_states[CPUStates::kSystem_]) +
         std::stol(cpu_states[CPUStates::kIRQ_]) +
         std::stol(cpu_states[CPUStates::kSoftIRQ_]) +
         std::stol(cpu_states[CPUStates::kSteal_]);
}

long LinuxParser::IdleJiffies() {
  vector<string> cpu_states = CpuUtilization();

  return std::stol(cpu_states[CPUStates::kIdle_]) +
         std::stol(cpu_states[CPUStates::kIOwait_]);
}

vector<string> LinuxParser::CpuUtilization() {
  std::ifstream stream(kProcDirectory + kStatFilename);

  string line;

  vector<string> cpu_states{"0", "0", "0", "0", "0", "0", "0", "0", "0", "0"};

  if (stream.is_open()) {
    std::getline(stream, line);

    std::istringstream linestream(line);

    string temp;
    linestream >> temp;
    cpu_states.clear();

    for (int i = 0; i <= CPUStates::kGuestNice_; i++) {
      string value;
      linestream >> value;
      cpu_states.push_back(value);
    }
  }

  return cpu_states;
}

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

string LinuxParser::Uid(int pid) {
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

string LinuxParser::User(int pid) {
  string user = Uid(pid);

  std::ifstream pwdstream(kPasswordPath);

  string username = "";

  if (pwdstream.is_open()) {
    string pwdline;

    while (std::getline(pwdstream, pwdline)) {
      std::replace(pwdline.begin(), pwdline.end(), ':', ' ');

      string uname, pwd, uid;

      std::istringstream pwdlinestream(pwdline);
      pwdlinestream >> uname >> pwd >> uid;

      if (uid == user) {
        username = uname;
        break;
      }
    };
  }

  return username;
}

long LinuxParser::UpTime(int pid) {
  string uptime = "0";

  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatFilename);

  if (stream.is_open()) {
    string line;
    std::getline(stream, line);

    std::istringstream linestream(line);

    string starttime;

    for (int i = 1; i <= 22; i++) {
      linestream >> starttime;
    }

    uptime = starttime;
  }

  return std::stol(uptime);
}
