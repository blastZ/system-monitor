#include "../include/format.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

using std::string;
using std::vector;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds [[maybe_unused]]) { return string(); }

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