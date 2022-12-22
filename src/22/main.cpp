// Advent of Code day 22 -- binary aoc22.22
// --- Day 22: Monkey Map ---

#include <iostream>
#include <libaoc.h>
#include "monkey_cube.h"
#include "monkey_map.h"

int main(int argc, const char *argv[]){
  if (argc > 4 || argc < 3) {
      std::cout << "usage: " << argv[0] << " [filename|input] [part] {[expected result]}" << std::endl;
    return 1;
  }
  using namespace aoc;
	using namespace aoc::monkey;

  std::string filename = argv[1];
  int part = std::stoi(argv[2]);
  bool do_test = argc > 3;
  auto result = static_cast<ind_t>((argc > 3) ? std::stoll(argv[3]) : 0);
  ind_t answer = 3;

  auto blocks = read_blocks(filename);

	if (blocks.size() != 2) throw std::runtime_error("Wrong input file?");

  if (1 == part){
	  aoc::monkey::map::Runner runner(blocks[0], blocks[1][0]);
		answer = runner.password();
		std::cout << runner;
  } else {
	  aoc::monkey::cube::Runner runner(blocks[0], blocks[1][0]);
		answer = runner.password();
		std::cout << runner;
	}

  /* Parse input string or file to produce 'answer' */

  std::cout << (do_test ? "Test " : "Part ") << part << " results in " << answer << "\n";

  /* Compare the expected result and indicate failure for CTest */
  return (do_test && result != answer) ? 1 : 0;
}
