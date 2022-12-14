#include "read.h"
#include <regex>

//using namespace aoc;
//

std::vector<std::string> aoc::read_block(std::fstream & fs){
	if (!fs.is_open()) {
	  std::string msg = "The provided file input stream is not open";
	  throw std::runtime_error(msg);
	}
	std::vector<std::string> output;
	std::string line;
	while (getline(fs, line) && !line.empty()) {
		output.push_back(line);
	}
	return output;
}

std::vector<std::string> aoc::read_lines(std::fstream & fs){
	if (!fs.is_open()) {
	  std::string msg = "The provided file input stream is not open";
	  throw std::runtime_error(msg);
	}
	std::vector<std::string> output;
	std::string line;
	while (getline(fs, line)) {
		output.push_back(line);
	}
	return output;
}

std::vector<std::string> aoc::read_lines(std::string & filename){
	std::fstream fs;
	fs.open(filename, std::ios::in);
	if (!fs.is_open()) {
	  std::string msg = "The provided filename ";
	  msg += filename;
	  msg += " does not exist!";
	  throw std::runtime_error(msg);
	}
	return aoc::read_lines(fs);
}

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

std::vector<std::string> aoc::line_to_columns(const std::string & line){
	std::regex column_regex("(\\w)+");

	auto col_begin = std::sregex_iterator(line.begin(), line.end(), column_regex);
	auto col_end = std::sregex_iterator();
	auto col_count = std::distance(col_begin, col_end);

	std::vector<std::string> out;
	out.reserve(col_count);
	for (auto i = col_begin; i != col_end; ++i){
		std::smatch match = *i;
		out.push_back(match.str());
	}

	return out;
}
