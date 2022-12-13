// Advent of Code day 13 -- binary aoc22.13

#include <iostream>
#include <libaoc.h>
#include <numeric>
#include "packet.h"

int main(int argc, const char *argv[]){
  if (argc > 4 || argc < 3) {
      std::cout << "usage: " << argv[0] << " [filename|input] [part] {[expected result]}" << std::endl;
    return 1;
  }

  std::string filename = argv[1];
  int part = std::stoi(argv[2]);
  bool do_test = argc > 3;
  int result = (argc > 3) ? std::stoi(argv[3]) : -1;

	auto pair_parser = [](const std::vector<std::string> & block){
		return std::make_pair(block[0], block[1]);
	};
  auto pairs = aoc::read_blocks<std::pair<std::string, std::string>>(filename, pair_parser);


	int answer = -3;
	if (part == 1){
    std::vector<aoc::packet::TwoPacket> packets;
    packets.reserve(pairs.size());
    std::transform(pairs.begin(), pairs.end(), std::back_inserter(packets),
                   [](const auto & pair){return aoc::packet::TwoPacket(pair);});

    std::vector<int> index(packets.size(), 0);
		std::iota(index.begin(), index.end(), 1);

		answer = std::transform_reduce(packets.begin(), packets.end(), index.begin(),
                                   0, [](int a, int b){return a + b;},
                                   [](const auto & packet, const auto & i){return (packet.ok() ? i : 0);}
                                   );
	} else {
    std::vector<aoc::packet::Packet> packets;
    packets.reserve(2 * pairs.size());
    for (auto & p: pairs) {
      packets.push_back(aoc::packet::from_string(p.first));
      packets.push_back(aoc::packet::from_string(p.second));
    }
    // add the divider packets
    packets.push_back(aoc::packet::from_string("[[2]]"));
    packets.push_back(aoc::packet::from_string("[[6]]"));
    // the last two packets are the dividers --
    std::vector<aoc::packet::Packet> divs{packets[packets.size()-2], packets[packets.size()-1]};
    // sort all packets
    std::sort(packets.begin(), packets.end(), [](auto & a, auto & b){return (a <=> b) == std::weak_ordering::less;});
    // find the dividers
    std::vector<int> positions;
    std::transform(divs.begin(), divs.end(), std::back_inserter(positions), [&](auto & x){
      return static_cast<int>(std::distance(packets.begin(), std::find(packets.begin(), packets.end(), x))) + 1;
    });
    answer = positions[0]*positions[1];
  }


  /* Parse input string or file to produce 'answer' */

  std::cout << (do_test ? "Test " : "Part ") << part << " results in " << answer << "\n";

  /* Compare the expected result and indicate failure for CTest */
  return (do_test && result != static_cast<int>(answer)) ? 1 : 0;
}
