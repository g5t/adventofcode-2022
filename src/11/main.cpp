// Advent of Code day 11 -- binary aoc22.11

#include <iostream>
#include <sstream>
#include <libaoc.h>

#include "monkey.h"

using uint_t = unsigned long long;

uint_t find_end(const std::string & line, const std::string & search){
	auto index = line.find(search);
	if (index == std::string::npos) throw std::runtime_error("Search string not found");
	return index + search.size();
}

std::string find_end_str(const std::string & line, const std::string & search){
	return line.substr(find_end(line, search));
}

uint_t stoui(const std::string & line){
	return static_cast<uint_t>(std::stoul(line));
}

int main(int argc, const char *argv[]){
  if (argc > 4 || argc < 3) {
      std::cout << "usage: " << argv[0] << " [filename|input] [part] {[expected result]}" << std::endl;
    return 1;
  }

  std::string filename = argv[1];
  int part = std::stoi(argv[2]);
  bool do_test = argc > 3;
  uint_t result = (argc > 3) ? stoui(argv[3]) : 0;


	auto make_monkey = [](const std::vector<std::string>& lines){
		std::stringstream swc(find_end_str(lines[1], "items: "));
		std::vector<uint_t> worry;
		while (swc.good()){
		  std::string s;
			getline(swc, s, ',');
			worry.push_back(stoui(s));
		}

		aoc::monkey::op op;
	  uint_t opv{0};
		if (lines[2].find("old * old") != std::string::npos){
			op = aoc::monkey::op::square;
		} else {
			auto opx = find_end(lines[2], "old ");
		  switch (lines[2][opx]){
		  case '*': op = aoc::monkey::op::multiply; break;
		  case '+': op = aoc::monkey::op::add; break;
		  default:
		  	throw std::runtime_error("Unknown Monkey operation");
		  }
			opv = stoui(lines[2].substr(opx+1));
		}
		auto mtv = stoui(find_end_str(lines[3], "divisible by "));
		auto tbv = stoui(find_end_str(lines[4], "monkey "));
		auto fbv = stoui(find_end_str(lines[5], "monkey "));

		return aoc::monkey::Monkey(worry, std::make_pair(op, opv), mtv, std::make_pair(tbv, fbv));
	};


	auto monkeys = aoc::read_blocks<aoc::monkey::Monkey<uint_t>>(filename, make_monkey);
	for (const auto & m: monkeys) std::cout << m;

	auto troop = aoc::monkey::Troop<uint_t>(std::move(monkeys), part==1?3:0, part==1?1:0);
	
	uint_t rounds{part==1 ? 20u : 10000u};
	for (uint_t i=0; i<rounds; ++i){
		if (i % 1000 == 0 || 1 == i || 20 == i) std::cout << i << " rounds: " << troop;
	  troop.run_round();
	}
	auto answer = troop.business();

  /* Parse input string or file to produce 'answer' */

  std::cout << (do_test ? "Test " : "Part ") << part << " results in " << answer << "\n";

  /* Compare the expected result and indicate failure for CTest */
  return (do_test && result != answer) ? 1 : 0;
}
