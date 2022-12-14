// Advent of Code day 14 -- binary aoc22.14

#include <sstream>
#include <iostream>
#include <libaoc.h>
#include "hourglass.h"

int main(int argc, const char *argv[]){
  if (argc > 4 || argc < 3) {
      std::cout << "usage: " << argv[0] << " [filename|input] [part] {[expected result]}" << std::endl;
    return 1;
  }

  std::string filename = argv[1];
  int part = std::stoi(argv[2]);
  bool do_test = argc > 3;
  int result = (argc > 3) ? std::stoi(argv[3]) : -1;

	int answer = -3;

	auto strings = aoc::read_lines(filename);
	auto glass = aoc::sand::from_strings(strings, part);

	answer = glass.run();

  std::stringstream image_name;
  image_name << (do_test ? "Test" : "Part") << "_" << part << ".bmp";
  glass.write_image(image_name.str());

  std::cout << (do_test ? "Test " : "Part ") << part << " results in " << answer << "\n";

  /* Compare the expected result and indicate failure for CTest */
  return (do_test && result != static_cast<int>(answer)) ? 1 : 0;
}
