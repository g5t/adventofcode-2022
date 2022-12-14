#ifndef AOC_READ_H
#define AOC_READ_H

#include <concepts>
#include <functional>

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

namespace aoc {

std::vector<std::string> read_block(std::fstream &);

std::vector<std::vector<int>> read_vector_of_vector_of_ints(const std::string & filename);

std::vector<std::string> line_to_columns(const std::string & line);

std::vector<std::string> read_lines(std::fstream &);
std::vector<std::string> read_lines(std::string &);

template<class T, class F>
requires std::invocable<F&, std::string &>
std::vector<T> read_vector_from_stream(std::fstream & fs, F fn){
  std::vector<T> output;
  std::string line;
  while(getline(fs, line)){
    if (!line.empty()){
			output.push_back(fn(line));
    }
  }
	return output;
}

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
	return read_vector_from_stream<T>(fs, fn);
}


template<class T, class F>
requires std::invocable<F&, std::vector<std::string> &>
T interpret_block(std::fstream & fs, F fn){
	std::vector<std::string> block;
	std::string line;
	while (getline(fs, line) && !line.empty()) {
		block.push_back(line);
	}
	return fn(block);
}

template<class T, class F>
std::vector<T> interpret_blocks(std::fstream& fs, F fn){
  std::vector<T> output;
	while (!fs.eof()) output.push_back(interpret_block<T>(fs, fn));
	return output;
}

template<class T, class F>
std::vector<T>
read_blocks(const std::string & filename, F fn){
	std::fstream fs;
  fs.open(filename, std::ios::in);
  if (!fs.is_open()) {
    std::string msg = "The provided filename ";
    msg += filename;
    msg += " does not exist!";
    throw std::runtime_error(msg);
  }
	return interpret_blocks<T>(fs, fn);
}

} // namespace aoc



#endif
