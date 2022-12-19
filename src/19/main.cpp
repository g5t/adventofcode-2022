// Advent of Code day 19 -- binary aoc22.19
// --- Day 19: Not Enough Minerals ---

#include <iostream>
#include <libaoc.h>
#include "robots.h"

int main(int argc, const char *argv[]){
  if (argc > 4 || argc < 3) {
      std::cout << "usage: " << argv[0] << " [filename|input] [part] {[expected result]}" << std::endl;
    return 1;
  }
  using namespace aoc;
	using namespace aoc::robots;

  std::string filename = argv[1];
  int part = std::stoi(argv[2]);
  bool do_test = argc > 3;
  auto result = static_cast<i_t>((argc > 3) ? std::stoll(argv[3]) : 0);
  auto answer = result;

  auto blueprints = read_vector_from_lines<std::shared_ptr<Blueprint>>(filename, blueprint_from_string);

//  auto fm = Foreman(blueprints[0], 24);
//  std::cout << "We found " << fm.geodes() << " geodes!\n";
//  if (fm.geodes() < 1000) return 0;

	std::vector<Foreman> foremen;


  if (1 == part){
    for (const auto & blueprint: blueprints) foremen.emplace_back(blueprint, 24);
	  i_t count{0};
		answer = 0;
		for (const auto & foreman: foremen) answer += ++count * foreman.geodes();
  } else {
    // Only the firs three inputs matter (and the test dataset only has two)
    if (!do_test) blueprints.erase(blueprints.begin()+3, blueprints.end());
    for (const auto & blueprint: blueprints) foremen.emplace_back(blueprint, 32);
    answer = 1;
    for (const auto & foreman: foremen) answer *= foreman.geodes();
  }

  /* Parse input string or file to produce 'answer' */

  std::cout << (do_test ? "Test " : "Part ") << part << " results in " << answer << "\n";

  /* Compare the expected result and indicate failure for CTest */
  return (do_test && result != answer) ? 1 : 0;
}
