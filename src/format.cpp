#include "../include/format.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

using std::string;
using std::vector;

string Format::ElapsedTime(long seconds) {
  // HH:MM:SS
  int hh = seconds / 3600;
  int mm = seconds % 3600 / 60;
  int ss = seconds % 3600 % 60;

  return std::to_string(hh) + ":" + std::to_string(mm) + ":" +
         std::to_string(ss);
}

vector<string> Format::GetLineListFromStream(std::istream& stream,
                                             char delimiter) {
  vector<string> line_list;

  while (stream.good()) {
    string line;

    std::getline(stream, line, delimiter);

    line_list.push_back(line);
  }

  return line_list;
}

vector<string> Format::GetLineListFromString(string str, char delimiter) {
  std::stringstream str_stream(str);

  return GetLineListFromStream(str_stream, delimiter);
}

vector<string> Format::GetLineListFromFile(string file_path, char delimiter) {
  std::ifstream file_stream;

  file_stream.open(file_path);

  if (file_stream.is_open()) {
    return GetLineListFromStream(file_stream, delimiter);
  }

  return vector<string>{};
}