// Advent of Code day 24 -- binary aoc22.24
// --- Day 24: Blizzard Basin ---

#include <iostream>
#include <libaoc.h>
#include "winds.h"

int main(int argc, const char *argv[]){
  if (argc > 4 || argc < 3) {
      std::cout << "usage: " << argv[0] << " [filename|input] [part] {[expected result]}" << std::endl;
    return 1;
  }
  using namespace aoc;
	using namespace aoc::winds;

  std::string filename = argv[1];
  int part = std::stoi(argv[2]);
  bool do_test = argc > 3;
  auto result = static_cast<ind_t>((argc > 3) ? std::stoll(argv[3]) : -1);
  ind_t answer = -3;

  auto lines = read_lines(filename);
  Walker walker(lines);
  if (1 == part){
		answer = walker.walk();
  } else {
	  answer = walker.triple();
	}

  /* Parse input string or file to produce 'answer' */

  std::cout << (do_test ? "Test " : "Part ") << part << " results in " << answer << "\n";

  /* Compare the expected result and indicate failure for CTest */
  return (do_test && result != answer) ? 1 : 0;
}
