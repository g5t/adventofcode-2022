// Advent of Code day 17 -- binary aoc22.17
// --- Day 17: Pyroclastic Flow ---

#include <iostream>
#include <libaoc.h>
#include "tetrastack.h"

int main(int argc, const char *argv[]){
  if (argc > 4 || argc < 3) {
      std::cout << "usage: " << argv[0] << " [filename|input] [part] {[expected result]}" << std::endl;
    return 1;
  }
  using namespace aoc;
	using namespace aoc::tetra;

  std::string filename = argv[1];
  int part = std::stoi(argv[2]);
  bool do_test = argc > 3;
  auto result = static_cast<Shape::ind_t>((argc > 3) ? std::stoll(argv[3]) : -1);

	std::fstream fs;
  fs.open(filename, std::ios::in);
	auto is_file = fs.is_open();

	std::string input;
	if (is_file) {
		getline(fs, input);
	} else {
		input = filename;
	}

  std::vector<shape> shapes{shape::horizontal, shape::plus, shape::backel, shape::vertical, shape::square};
	std::vector<direction> jets;
	jets.reserve(input.size());

	for (const auto & c: input) {
    switch (c) {
      case '<':
        jets.push_back(direction::left);
        break;
      case '>':
        jets.push_back(direction::right);
        break;
      default:
        std::cout << "Unknown jet direction " << c << " is skipped.\n";
    }
  }

	size_t rounds = 1==part ? 2022 : 1'000'000'000'000;
	auto stack = Stack(rounds, shapes, jets);

//  auto answer = stack.run(rounds);
  auto answer = stack.pattern_run(rounds);
//  if (1 == part) {
//    auto answer1 = Stack(rounds, shapes, jets).run(rounds);
//    if (answer != answer1) std::cout << "The two methods disagree!" << std::endl;
//  }

  /* Parse input string or file to produce 'answer' */

  std::cout << (do_test ? "Test " : "Part ") << part << " results in " << answer << "\n";

  /* Compare the expected result and indicate failure for CTest */
  return (do_test && result != answer) ? 1 : 0;
}
