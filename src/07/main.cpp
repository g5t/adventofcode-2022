// Advent of Code day 07 -- binary aoc22.07

#include <iostream>
#include <libaoc.h>

int main(int argc, const char *argv[]){
  if (argc > 4 || argc < 3) {
      std::cout << "usage: " << argv[0] << " [filename|input] [part] {[expected result]}" << std::endl;
    return 1;
  }

  std::string filename = argv[1];
  int part = std::stoi(argv[2]);
  bool do_test = argc > 3;
  int result = (argc > 3) ? std::stoi(argv[3]) : -1;

  std::fstream fs;
  fs.open(filename, std::ios::in);
  auto is_file = fs.is_open();

  std::string input;
  if (is_file) {
    getline(fs, input);
  } else {
    input = filename;
  }

	auto ffs = std::make_shared<aoc::FakeFS>();
	std::weak_ptr<aoc::FakeFS> root = ffs;
	aoc::fakefilesystem_inserter(ffs, aoc::read_lines(fs));
	auto rfs = root.lock();
	rfs->walk_du();
	//std::cout << *rfs;

  /* Parse input string or file to produce 'answer' */
	int answer;
	if (part == 1) {
		answer = static_cast<int>(rfs->sum_children_smaller_than(100001));
	} else {
		aoc::FakeFS::ind_t disk_space{70000000}, required_space{30000000};
		auto occupied_space = rfs->size();
		auto available_space = disk_space - occupied_space;
		if (available_space > required_space) throw std::runtime_error("No work! Something is wrong");
		auto to_free = required_space - available_space;
		answer = static_cast<int>(rfs->optimal_child_size(to_free, occupied_space));
	}

  std::cout << (do_test ? "Test " : "Part ") << part << " results in " << answer << "\n";

  /* Compare the expected result and indicate failure for CTest */
  return (do_test && result != static_cast<int>(answer)) ? 1 : 0;
}
