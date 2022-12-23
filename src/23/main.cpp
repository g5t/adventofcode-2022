// Advent of Code day 22 -- binary aoc22.22
// --- Day 22: Monkey Map ---

#include <iostream>
#include <libaoc.h>
#include "grove.h"

int main(int argc, const char *argv[]){
  if (argc > 4 || argc < 3) {
      std::cout << "usage: " << argv[0] << " [filename|input] [part] {[expected result]}" << std::endl;
    return 1;
  }
  using namespace aoc;
	using namespace aoc::grove;

  std::string filename = argv[1];
  int part = std::stoi(argv[2]);
  bool do_test = argc > 3;
  auto result = static_cast<int64_t>((argc > 3) ? std::stoll(argv[3]) : 0);
  int64_t answer = 3;

  auto board = read_lines(filename);

	Planters<int64_t> troop(board);

  if (1 == part){
	  troop.diffuse(10);
		answer = troop.empty_ground();
	} else {
	  // find the round number where everyone is happy
	  answer = troop.diffuse();
		std::cout << "Final configuration\n" << troop;
	}

  /* Parse input string or file to produce 'answer' */

  std::cout << (do_test ? "Test " : "Part ") << part << " results in " << answer << "\n";

  /* Compare the expected result and indicate failure for CTest */
  return (do_test && result != answer) ? 1 : 0;
}
