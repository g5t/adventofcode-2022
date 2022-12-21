// Advent of Code day 21 -- binary aoc22.21
// --- Day 21: Monkey Math ---

#include <iostream>
#include <libaoc.h>
#include "monkey_math.h"

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
  int64_t result = (argc > 3) ? std::stoll(argv[3]) : -1;
  int64_t answer = -3;

  auto program = read_lines(filename);

  auto troop = Troop(program, part);

  if (1 == part){
    troop.execute();
    auto root_ptr = troop.at("root");
    answer = root_ptr->value();
  } else {
    answer = troop.solve();
  }

  /* Parse input string or file to produce 'answer' */

  std::cout << (do_test ? "Test " : "Part ") << part << " results in " << answer << "\n";

  /* Compare the expected result and indicate failure for CTest */
  return (do_test && result != answer) ? 1 : 0;
}
