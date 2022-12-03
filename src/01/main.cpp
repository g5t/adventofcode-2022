#include <algorithm>
#include <iostream>
#include <libaoc.h>
#include <numeric>

int main(int argc, const char *argv[]){
  if (argc > 4 || argc < 3) {
    std::cout << "usage: " << argv[0] << " [filename] [part] {[expected result]}" << std::endl;
    return 1;
  }

  std::string filename = argv[1];
  int part = std::stoi(argv[2]);
  int test_value = (argc > 3) ? std::stoi(argv[3]) : -1;

  std::vector<std::vector<int>> vvi = aoc::read_vector_of_vector_of_ints(filename);

  std::vector<int> sums;
  sums.reserve(vvi.size());
  for (auto & vi: vvi) sums.push_back(std::reduce(vi.begin(), vi.end()));

  std::partial_sort(sums.begin(), sums.begin()+3, sums.end(), std::greater());
	int result;
	if (part < 2){
	  std::cout << "The elf with the most Calories is carrying ";
    result = sums[0];
	} else {
	  std::cout << "The three elves with the most Calories are carrying ";
		result = std::reduce(sums.begin(), sums.begin()+3);
	}
  std::cout << result << " calories";

	return (test_value > 0) ? (test_value == result) ? 0 : 1 : 0;
}
