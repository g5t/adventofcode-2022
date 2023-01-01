// Advent of Code day 25 -- binary aoc22.25
// --- Day 25: Full of Hot Air ---

#include <iostream>
#include <libaoc.h>
#include "hotair.h"

int main(int argc, const char *argv[]){
  if (argc > 4 || argc < 3) {
      std::cout << "usage: " << argv[0] << " [filename|input] [part] {[expected result]}" << std::endl;
    return 1;
  }
  using namespace aoc;
	using namespace aoc::hotair;

  std::string filename = argv[1];
  int part = std::stoi(argv[2]);
  bool do_test = argc > 3;
  ind_t result = (argc > 3) ? static_cast<ind_t>(std::stoll(argv[3])) : -1;
  ind_t answer = -3;

	auto snafus = read_vector_from_lines<SNAFU>(filename, [](const std::string & line){return SNAFU(line);});

  if (1 == part){
	  auto snafu_sum = std::reduce(snafus.begin(), snafus.end(), SNAFU(0));
		std::cout << "Total sum is " << snafu_sum << " (decimal: " << snafu_sum.dec() << ")\n";
		answer = snafu_sum.dec();
  }


  /* Parse input string or file to produce 'answer' */

  std::cout << (do_test ? "Test " : "Part ") << part << " results in " << answer << "\n";

  /* Compare the expected result and indicate failure for CTest */
	return (do_test && result != answer) ? 1 : 0;
}
