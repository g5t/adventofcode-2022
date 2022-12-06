#include <iostream>
#include <libaoc.h>


int main(int argc, const char *argv[]){
  if (argc > 4 || argc < 3) {
    std::cout << "usage: " << argv[0] << " [filename] [part] {[expected result]}" << std::endl;
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

	auto n_different = [](unsigned n, auto ptr){
		for (unsigned i=0; i<n-1; ++i)
		for (unsigned j=i+1; j<n; ++j)
		if (*(ptr-i) == *(ptr-j)) return false;
		return true;
	};

	// Read and run the instructions:
	unsigned length = (part == 1) ? 4 : 14;
	unsigned answer = length - 1u;

	auto s = input.begin();
	for (; answer < input.size() && !n_different(length, s+answer); ++answer);
	++answer; // convert to 1-based indexing for AoC comparison


	std::cout << (do_test ? "Test " : "Part ") << part << " results in " << answer << "\n";

	return (do_test && result != static_cast<int>(answer)) ? 1 : 0;

}
