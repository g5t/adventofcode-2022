#include <iostream>
#include "read.h"

int main(int argc, const char *argv[]){
if (argc != 2) { std::cout << "usage: " << argv[0] << " [filename]" << std::endl; return 1; }

auto vvi = aoc::read_vector_of_vector_of_ints(argv[1]);

for (auto & vi : vvi){
  for (auto & i: vi) std::cout << i << '\n';
	std::cout << '\n';
}

return 0;
}
