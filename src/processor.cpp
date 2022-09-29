#include "../include/processor.h"

#include <chrono>
#include <string>
#include <thread>

#include "../include/linux_parser.h"

float Processor::Utilization() {
  long total = LinuxParser::Jiffies();
  long active = LinuxParser::ActiveJiffies();

  std::this_thread::sleep_for(std::chrono::milliseconds(10));

  long total_now = LinuxParser::Jiffies();
  long active_now = LinuxParser::ActiveJiffies();

  int totald = total_now - total;
  int actived = active_now - active;

  return actived * 1.0f / totald;
}