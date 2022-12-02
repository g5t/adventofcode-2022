#ifndef AOC_READ_H
#define AOC_READ_H

#include <concepts>
#include <functional>

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

namespace aoc {
std::vector<std::vector<int>> read_vector_of_vector_of_ints(const std::string & filename);

template<class T, class F>
requires std::invocable<F&, std::string &>
std::vector<T> read_vector_from_lines(const std::string & filename, F fn){
  std::fstream fs;
  fs.open(filename, std::ios::in);
  if (!fs.is_open()) {
    std::string msg = "The provided filename ";
    msg += filename;
    msg += " does not exist!";
    throw std::runtime_error(msg);
  }
  
  std::vector<T> output;
  std::string line;
  while(getline(fs, line)){
    if (!line.empty()){
			output.push_back(fn(line));
    }
  }

return output;
}

}



#endif
