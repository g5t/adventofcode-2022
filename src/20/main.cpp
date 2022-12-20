// Advent of Code day 20 -- binary aoc22.20
// --- Day 20: Grove Positioning System ---

#include <iostream>
#include <libaoc.h>
#include "crypt.h"

int main(int argc, const char *argv[]){
  if (argc > 4 || argc < 3) {
      std::cout << "usage: " << argv[0] << " [filename|input] [part] {[expected result]}" << std::endl;
    return 1;
  }
  using namespace aoc;
	using namespace aoc::crypt;

  std::string filename = argv[1];
  int part = std::stoi(argv[2]);
  bool do_test = argc > 3;
  auto result = static_cast<int64_t>((argc > 3) ? std::stoi(argv[3]) : -1);
  auto answer = result + 3;

	auto read_ints = [](const std::string & x){return static_cast<int64_t>(std::stoi(x));};

	int64_t key{1==part ? 1 : 811589153}, rounds{1==part ? 1 : 10};
  auto mixer = Mixer(read_vector_from_lines<int64_t>(filename, read_ints), key, rounds);
  answer = mixer.coordinate_sum();

  /* Parse input string or file to produce 'answer' */

  std::cout << (do_test ? "Test " : "Part ") << part << " results in " << answer << "\n";

  /* Compare the expected result and indicate failure for CTest */
  return (do_test && result != answer) ? 1 : 0;
}
