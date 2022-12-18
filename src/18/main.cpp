// Advent of Code day 18 -- binary aoc22.18
// --- Day 18: Boiling Boulders ---

#include <iostream>
#include <libaoc.h>
#include "cuboid.h"

int main(int argc, const char *argv[]){
  if (argc > 4 || argc < 3) {
      std::cout << "usage: " << argv[0] << " [filename|input] [part] {[expected result]}" << std::endl;
    return 1;
  }
  using namespace aoc;
	using namespace aoc::cuboid;

  std::string filename = argv[1];
  int part = std::stoi(argv[2]);
  bool do_test = argc > 3;
  auto result = static_cast<Drop::ind_t>((argc > 3) ? std::stoi(argv[3]) : -1);
  Drop::ind_t answer = -3;


	auto lines = read_lines(filename);
	auto drop = Drop(lines);

  if (1 == part){
    answer = drop.surface_area();
  } else {
	  answer = drop.flood_surface_area();
	}

  /* Parse input string or file to produce 'answer' */

  std::cout << (do_test ? "Test " : "Part ") << part << " results in " << answer << "\n";

  /* Compare the expected result and indicate failure for CTest */
  return (do_test && result != static_cast<int>(answer)) ? 1 : 0;
}
