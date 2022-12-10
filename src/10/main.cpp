// Advent of Code day 10 -- binary aoc22.10

#include <iostream>
#include <libaoc.h>

#include "crt.h"

int main(int argc, const char *argv[]){
  if (argc > 4 || argc < 3) {
      std::cout << "usage: " << argv[0] << " [filename|input] [part] {[expected result]}" << std::endl;
    return 1;
  }

  std::string filename = argv[1];
  int part = std::stoi(argv[2]);
  bool do_test = argc > 3;
  int result = (argc > 3) ? std::stoi(argv[3]) : -1;

	auto instr_parser = [](const std::string & line){
		if (line.starts_with("addx"))
			return std::make_pair(aoc::crt::instruction::addx, std::stoi(line.substr(5)));
		return std::make_pair(aoc::crt::instruction::noop, 0);
	};

	auto instr = aoc::read_vector_from_lines<std::pair<aoc::crt::instruction, int>>(filename, instr_parser);

	int answer = result;
	if (part == 1){
	  aoc::crt::CPU cpu;
	  answer = cpu.run(instr);
	} else {
	  aoc::crt::CRT crt;
		answer = crt.run(instr);
		std::cout << crt << "\n";
	}

  /* Parse input string or file to produce 'answer' */

  std::cout << (do_test ? "Test " : "Part ") << part << " results in " << answer << "\n";

  /* Compare the expected result and indicate failure for CTest */
  return (do_test && result != static_cast<int>(answer)) ? 1 : 0;
}
