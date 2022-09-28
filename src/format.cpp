#include "../include/format.h"

string Format::ElapsedTime(long seconds) {
  // HH:MM:SS
  int hh = seconds / 3600;
  int mm = seconds % 3600 / 60;
  int ss = seconds % 3600 % 60;

  return std::to_string(hh) + ":" + std::to_string(mm) + ":" +
         std::to_string(ss);
}