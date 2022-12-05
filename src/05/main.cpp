#include <algorithm>
#include <iostream>
#include <libaoc.h>
#include <numeric>

#include "stacks.h"


int main(int argc, const char *argv[]){
  if (argc > 4 || argc < 3) {
    std::cout << "usage: " << argv[0] << " [filename] [part] {[expected result]}" << std::endl;
    return 1;
	}

  std::string filename = argv[1];
  int part = std::stoi(argv[2]);
	bool do_test = argc > 3;
	std::string result = (argc > 3) ? std::string(argv[3]) : std::string();

	std::fstream fs;
  fs.open(filename, std::ios::in);
  if (!fs.is_open()) {
    std::string msg = "The provided filename ";
    msg += filename;
    msg += " does not exist!";
    throw std::runtime_error(msg);
  }

	auto stacks = aoc::stacks::read<char>(aoc::read_block(fs), [](char c){return c;});

	auto line_instruction = [](std::string & line){
	  // Line is of the form "move N from X to Y"
		auto cols = aoc::line_to_columns(line);
		auto n = static_cast<size_t>(std::stoi(cols[1]));
		// input instruction columns are 1-based instead of 0-based
		auto x = static_cast<size_t>(std::stoi(cols[3])) - 1;
		auto y = static_cast<size_t>(std::stoi(cols[5])) - 1;
		return std::make_tuple(n, x, y);
	};

	// Read and run the instructions:
	std::string line;
	if (part == 1) {
	while (getline(fs, line)) {
		//std::cout << stacks << "\n";
		auto [n, x, y] = line_instruction(line);
		for (size_t i=0; i<n; ++i) stacks.move(x, y);
	}
	} else {
		while (getline(fs, line)) {
			auto [n, x, y] = line_instruction(line);
			stacks.move(n, x, y);
		}
	}

	std::vector<char> top = stacks.on_top();

	std::string tops = std::string(top.begin(), top.end());

	std::cout << (do_test ? "Test " : "Part ") << part << " results in " << tops << "\n";

	return (do_test && tops.compare(result)) ? 1 : 0;

}
