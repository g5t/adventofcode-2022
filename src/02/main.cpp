#include <algorithm>
#include <iostream>
#include <libaoc.h>
#include <numeric>
#include <map>

enum choice {r=1, p=2, s=3};
enum result {l=0, d=3, w=6};

choice str2choice(std::string && str) {
  switch (str[0]){
  case 'A': case 'X': return r;
  case 'B': case 'Y': return p;
  case 'C': case 'Z': return s;
  default: throw std::runtime_error("Unknown choice");
  }
}
result str2score(std::string && str) {
  switch (str[0]){
  case 'X': return l;
  case 'Y': return d;
  case 'Z': return w;
  default: throw std::runtime_error("Unknown intent");
  }
}

int main(int argc, const char *argv[]){
  if (argc != 3) { std::cout << "usage: " << argv[0] << " [filename] [part]" << std::endl; return 1; }

  std::string filename = argv[1];
	int part = std::stoi(argv[2]);

  std::map<std::pair<choice, choice>, result> decoder {
  	{{r,r},d}, {{r,p},l}, {{r,s},w},
  	{{p,r},w}, {{p,p},d}, {{p,s},l},
  	{{s,r},l}, {{s,p},w}, {{s,s},d}
	};

	std::map<std::pair<choice, result>, choice> intent {
	  {{r,l}, s}, {{r, d}, r}, {{r, w}, p},
	  {{p,l}, r}, {{p, d}, p}, {{p, w}, s},
	  {{s,l}, p}, {{s, d}, s}, {{s, w}, r},
	};

  auto first_parser = [&](std::string & line){
    auto their = str2choice(line.substr(0, line.find(" ")));
    // remove their choice and the space
    line.erase(0, line.find(" ") + 1);
    auto mine = str2choice(line.substr(0, line.find(" ")));
    auto score = decoder[std::make_pair(mine, their)];
    return static_cast<int>(mine) + static_cast<int>(score);
  };

	auto second_parser = [&](std::string & line){
    auto their = str2choice(line.substr(0, line.find(" ")));
    // remove their choice and the space
    line.erase(0, line.find(" ") + 1);
    auto score = str2score(line.substr(0, line.find(" ")));
    auto mine = intent[std::make_pair(their, score)];
    return static_cast<int>(mine) + static_cast<int>(score);
	};

  std::vector<int> scores;
	if (part == 2){
		scores = aoc::read_vector_from_lines<int>(filename, second_parser);

	} else {
		scores = aoc::read_vector_from_lines<int>(filename, first_parser);
	}

  auto total = std::reduce(scores.begin(), scores.end());

  std::cout << "Part " << part << " score is " << total << "\n";

  return 0;

}
