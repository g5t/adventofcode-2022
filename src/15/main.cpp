// Advent of Code day 15 -- binary aoc22.15

#include <iostream>
#include <libaoc.h>
#include "sensor.h"


int main(int argc, const char *argv[]){
  if (argc > 4 || argc < 3) {
      std::cout << "usage: " << argv[0] << " [filename|input] [part] {[expected result]}" << std::endl;
    return 1;
  }
	using namespace aoc::sensor;
	using namespace aoc;

  std::string filename = argv[1];
  int part = std::stoi(argv[2]);
  bool do_test = argc > 3;
  auto result = static_cast<Sensor::ind_t>((argc > 3) ? std::stoi(argv[3]) : -1);

	auto network = Network(read_vector_from_lines<Sensor>(filename, from_string));

  Sensor::ind_t answer = -3;
  if (1 == part){
		answer = network.count_excluded_with_y(do_test ? 10 : 2'000'000);
	} else {
	  answer = network.find_distress_beacon(do_test ? 20 : 4'000'000);
	}


  std::cout << (do_test ? "Test " : "Part ") << part << " results in " << answer << "\n";

  /* Compare the expected result and indicate failure for CTest */
  return (do_test && result != answer) ? 1 : 0;
}
