#include <algorithm>
#include <iostream>
#include <libaoc.h>
#include <numeric>
#include <map>
#include <bitset>

size_t char2priority(const char & cc){
	auto code = static_cast<size_t>(cc - 'A');
	if (code < 26) return code + 26u;
	code = static_cast<size_t>(cc - 'a');
	if (code < 26) return code;
	std::string msg = "Invalid code for character ";
	msg += cc;
	throw std::runtime_error(msg);
}

int restore_priority(std::bitset<52> store){
  auto val = store.to_ullong();
	int priority = 1;
	while (val >>= 1) ++priority;
	return priority;
}


int main(int argc, const char *argv[]){
  if (argc > 4 || argc < 3) {
    std::cout << "usage: " << argv[0] << " [filename] [part] {[expected result]}" << std::endl;
    return 1;
	}

  std::string filename = argv[1];
  int part = std::stoi(argv[2]);
  int test_value = (argc > 3) ? std::stoi(argv[3]) : -1;

  auto first_parser = [&](std::string & line){
    size_t count = line.size();
		size_t half = count >> 1;
		std::bitset<52> store(0);
		for (size_t i = 0; i < half; ++i) {
			store[char2priority(line[i])] = 1;
	  }
		for (size_t i = half; i < count; ++i){
		  auto priority = char2priority(line[i]);
			if (store[priority]){
			  return static_cast<int>(priority) + 1;
			}
		}
		return 0;
  };

	auto second_parser = [&](std::string & line){
	  std::bitset<52> store(0);
	  for (auto & c: line) store[char2priority(c)] = 1;
		return store;
	};

  std::vector<int> scores;
	if (part == 2){
    auto packs = aoc::read_vector_from_lines<std::bitset<52>>(filename, second_parser);
		for (size_t i=0; i<packs.size(); i+=3){
		  auto badge = packs[i] & packs[i+1] & packs[i+2];
			scores.push_back(restore_priority(badge));
		}
	} else {
		scores = aoc::read_vector_from_lines<int>(filename, first_parser);
	}

  auto total = std::reduce(scores.begin(), scores.end());

  std::cout << ((test_value > 0) ? "Test " : "Part ") << part << " total is " << total << "\n";

	return (test_value > 0) ? (test_value == total) ? 0 : 1 : 0;

}
