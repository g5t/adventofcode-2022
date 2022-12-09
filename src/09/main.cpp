// Advent of Code day 09 -- binary aoc22.09

#include <iostream>
#include <libaoc.h>
#include "rope.h"

int main(int argc, const char *argv[]){
  if (argc > 4 || argc < 3) {
      std::cout << "usage: " << argv[0] << " [filename|input] [part] {[expected result]}" << std::endl;
    return 1;
  }

  std::string filename = argv[1];
  int part = std::stoi(argv[2]);
  bool do_test = argc > 3;
  int result = (argc > 3) ? std::stoi(argv[3]) : -1;

	auto instruction_parser = [](const std::string & line){
	  aoc::points dir = aoc::points::left;
		switch (line[0]){
		case 'l': case 'L': dir = aoc::points::left; break;
		case 'r': case 'R': dir = aoc::points::right; break;
		case 'u': case 'U': dir = aoc::points::up; break;
		case 'd': case 'D': dir = aoc::points::down; break;
		}
		auto num = std::stoi(line.substr(2));
		return std::make_pair(dir, num);
	};

	auto instructions = aoc::read_vector_from_lines<std::pair<aoc::points, int>>(filename, instruction_parser);

	aoc::Rope rope(part == 1 ? 1u : 9u);

	for (const auto & move: instructions) rope.move(move.first, move.second);
	auto answer = static_cast<int>(rope.visit_count());

  /* Parse input string or file to produce 'answer' */

  std::cout << (do_test ? "Test " : "Part ") << part << " results in " << answer << "\n";

  /* Compare the expected result and indicate failure for CTest */
  return (do_test && result != static_cast<int>(answer)) ? 1 : 0;
}
