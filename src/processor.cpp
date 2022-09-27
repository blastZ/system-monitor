#include "../include/processor.h"

#include <chrono>
#include <string>
#include <thread>

#include "../include/format.h"

vector<int> Processor::GetTotalFromStatFile() {
  vector<string> stat_list = Format::GetLineListFromFile("/proc/stat");

  vector<string> cpu_stat_list =
      Format::GetLineListFromString(stat_list[0], ' ');

  cpu_stat_list.erase(cpu_stat_list.begin());
  cpu_stat_list.erase(cpu_stat_list.begin());

  int idle = std::stoi(cpu_stat_list[3]) + std::stoi(cpu_stat_list[4]);
  int non_idle = std::stoi(cpu_stat_list[0]) + std::stoi(cpu_stat_list[1]) +
                 std::stoi(cpu_stat_list[2]) + std::stoi(cpu_stat_list[5]) +
                 std::stoi(cpu_stat_list[6]) + std::stoi(cpu_stat_list[7]);
  int total = idle + non_idle;

  return vector<int>{total, idle};
}

float Processor::Utilization() {
  vector<int> pre = GetTotalFromStatFile();

  std::this_thread::sleep_for(std::chrono::milliseconds(100));

  vector<int> now = GetTotalFromStatFile();

  int totald = now[0] - pre[0];
  int idled = now[1] - pre[1];

  return (totald - idled) * 1.0f / totald;
}