#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <vector>

class Processor {
 public:
  float Utilization();

 private:
  std::vector<int> GetTotalFromStatFile();
};

#endif