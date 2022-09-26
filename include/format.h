#ifndef FORMAT_H
#define FORMAT_H

#include <string>
#include <vector>

using std::string;
using std::vector;

namespace Format {
string ElapsedTime(long times);

vector<string> GetLineListFromStream(std::istream& stream,
                                     char delimiter = '\n');

vector<string> GetLineListFromString(string str, char delimiter = '\n');

vector<string> GetLineListFromFile(string file_path, char delimiter = '\n');
}  // namespace Format

#endif