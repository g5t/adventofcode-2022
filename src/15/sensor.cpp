#include "sensor.h"
#include <regex>
#include <iostream>

aoc::sensor::Sensor aoc::sensor::from_string(const std::string & line){
  using namespace aoc::sensor;
  std::regex coord_regex("(x=(-?[0-9]+),\\s*y=(-?[0-9]+))");
  auto coord_begin = std::sregex_iterator(line.begin(), line.end(), coord_regex);
  auto coord_end = std::sregex_iterator();
  auto coord_count = std::distance(coord_begin, coord_end);

  std::vector<Sensor::coord_t> coords;
  coords.reserve(static_cast<size_t>(coord_count));
  for (auto i = coord_begin; i != coord_end; ++i){
    std::smatch match = *i;
    coords.emplace_back(std::stoi(match.str(2)), std::stoi(match.str(3)));
  }

	if (coords.size() != 2) std::cout << "Found " << coords.size() << " coordinates when 2 were expected.\n";

	return {coords[0], coords[1]};
}

