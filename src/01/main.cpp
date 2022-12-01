#include <algorithm>
#include <iostream>
#include <libaoc.h>
#include <numeric>

int main(int argc, const char *argv[]){
if (argc != 2) { std::cout << "usage: " << argv[0] << " [filename]" << std::endl; return 1; }

std::string filename = argv[1];

std::vector<std::vector<int>> vvi = aoc::read_vector_of_vector_of_ints(filename);

std::vector<int> sums;
sums.reserve(vvi.size());
for (auto & vi: vvi) sums.push_back(std::reduce(vi.begin(), vi.end()));

std::partial_sort(sums.begin(), sums.begin()+3, sums.end(), std::greater());
std::cout << "Find the Elf carrying the most Calories.\nHow many total Calories is that Elf carrying? " << sums[0] << std::endl;
std::cout << "Find the top three Elves carrying the most Calories.\nHow many Calories are those Elves carrying in total? " << std::reduce(sums.begin(), sums.begin()+3);

return 0;
}
