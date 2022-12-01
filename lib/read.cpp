#include "read.h"

//using namespace aoc;

std::vector<std::vector<int>> aoc::read_vector_of_vector_of_ints(const std::string & filename) {

std::fstream fs;
fs.open(filename, std::ios::in);
if (!fs.is_open()) {
  std::string msg = "The provided filename ";
  msg += filename;
  msg += " does not exist!";
  throw std::runtime_error(msg);
}

std::vector<std::vector<int>> output;
output.emplace_back();

std::string line;
while(getline(fs, line)){
  if (line.empty()){
    output.emplace_back();
  } else { 
    output.back().push_back(stoi(line));
  }
}

return output;
}
