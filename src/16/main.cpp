// Advent of Code day 16 -- binary aoc22.16
// Proboscidea Volcanium

#include <iostream>
#include <libaoc.h>
#include "steam.h"

int main(int argc, const char *argv[]){
  if (argc > 4 || argc < 3) {
      std::cout << "usage: " << argv[0] << " [filename|input] [part] {[expected result]}" << std::endl;
    return 1;
  }
  using namespace aoc;
  using namespace aoc::steam;

  std::string filename = argv[1];
  int part = std::stoi(argv[2]);
  bool do_test = argc > 3;
  int result = (argc > 3) ? std::stoi(argv[3]) : -1;
  int answer = -3;

  auto piperunner = from_strings(read_lines(filename));
  std::cout << piperunner << "\n";

  if (1 == part){
    answer = piperunner.optimal_plan(30);
  } else {
    answer = piperunner.elephant_plan(26);
  }

  /* Parse input string or file to produce 'answer' */

  std::cout << (do_test ? "Test " : "Part ") << part << " results in " << answer << "\n";

  /* Compare the expected result and indicate failure for CTest */
  return (do_test && result != static_cast<int>(answer)) ? 1 : 0;
}
