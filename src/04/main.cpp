#include <algorithm>
#include <iostream>
#include <libaoc.h>
#include <numeric>

#include "assignment.h"


int main(int argc, const char *argv[]){
  if (argc > 4 || argc < 3) {
    std::cout << "usage: " << argv[0] << " [filename] [part] {[expected result]}" << std::endl;
    return 1;
	}

  std::string filename = argv[1];
  int part = std::stoi(argv[2]);
  int test_value = (argc > 3) ? std::stoi(argv[3]) : -1;

	auto line_pair = [](std::string & line){
	  // Line is of the form ###-####,##-#####
	  auto comma = line.find(",");
		auto elf0 = aoc::assignment::Sections<int>(line.substr(0, comma));
		auto elf1 = aoc::assignment::Sections<int>(line.substr(comma+1, line.size()));
	  return std::make_pair(elf0, elf1);
	};

  auto first_parser = [&](std::string & line){
	  // Line is of the form ###-####,##-#####
	  auto comma = line.find(",");
		auto elf0 = aoc::assignment::Sections<int>(line.substr(0, comma));
		auto elf1 = aoc::assignment::Sections<int>(line.substr(comma+1, line.size()));

		return aoc::assignment::duplicated_effort(elf0, elf1) ? 1 : 0;
  };

	auto second_parser = [&](std::string & line){
		return aoc::assignment::overlaps(line_pair(line)) ? 1 : 0;
	};

	int count;
	if (part != 2){
		auto scores = aoc::read_vector_from_lines<int>(filename, first_parser);
		count = std::reduce(scores.begin(), scores.end());
	} else {
	  auto scores = aoc::read_vector_from_lines<int>(filename, second_parser);
		count = std::reduce(scores.begin(), scores.end());
	}

  std::cout << ((test_value > 0) ? "Test " : "Part ") << part << " count is " << count << "\n";

	return (test_value > 0) ? (test_value == count) ? 0 : 1 : 0;

}
