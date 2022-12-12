// Advent of Code day 12 -- binary aoc22.12

#include <iostream>
#include <libaoc.h>
#include "topo.h"

int main(int argc, const char *argv[]){
  if (argc > 4 || argc < 3) {
      std::cout << "usage: " << argv[0] << " [filename|input] [part] {[expected result]}" << std::endl;
    return 1;
  }

  std::string filename = argv[1];
  int part = std::stoi(argv[2]);
  bool do_test = argc > 3;
  int result = (argc > 3) ? std::stoi(argv[3]) : -1;


  std::fstream fs;
  fs.open(filename, std::ios::in);
	auto lines = aoc::read_lines(fs);


	int answer=-3;
	if (1 == part){
		auto topo = aoc::topo::Map(lines);
		auto a_path = topo.blaze();
		answer = static_cast<int>(a_path.size()) - 1; // number of steps is one less than length
	} else {
		auto hikes = aoc::topo::Hikes(lines);
		auto opt_path = hikes.optimal();
		answer = static_cast<int>(opt_path.size()) - 1;
	}

  /* Parse input string or file to produce 'answer' */

  std::cout << (do_test ? "Test " : "Part ") << part << " results in " << answer << "\n";

  /* Compare the expected result and indicate failure for CTest */
  return (do_test && result != static_cast<int>(answer)) ? 1 : 0;
}
