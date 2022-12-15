#include <algorithm>
#include <iostream>
#include <libaoc.h>
#include <numeric>

//#include "assignment.h"

namespace aoc::assignment {

template<class T>
class Sections{
	T f_;
	T l_;
public:
  Sections(T f, T l): f_(f), l_(l) {};

	// Construct from a string like "FFFF-LLL"
	Sections(const std::string & repr){
	 auto dash = repr.find("-"); 
	 f_ = std::stoi(repr.substr(0, dash));
	 l_ = std::stoi(repr.substr(dash+1, repr.size()));
	}

	T first() const {return f_;}
	T last() const {return l_;}

	bool contains(const Sections<T>& partner) const {
	  return (f_ <= partner.first() && l_ >= partner.last());
	}

	bool overlaps(const Sections<T>& partner) const {
	  auto pf = partner.first();
		auto pl = partner.last();
	  if (f_ <= pf && pf <= l_) return true;
		if (f_ <= pl && pl <= l_) return true;
		if (pf <= f_ && f_ <= pl) return true;
		if (pf <= l_ && l_ <= pl) return true;
		return false;
	}
};

template<class T>
bool duplicated_effort(const Sections<T>& a, const Sections<T>& b){
	return (a.contains(b) || b.contains(a));
}
template<class T>
bool duplicated_effort(const std::pair<Sections<T>, Sections<T>>  & team){
	return (team.first.contains(team.second) || (team.second.contains(team.first)));
}

template<class T>
bool overlaps(const std::pair<Sections<T>, Sections<T>> & team){
	return team.first.overlaps(team.second);
}


}

int main(int argc, const char *argv[]){
  if (argc > 4 || argc < 3) {
    std::cout << "usage: " << argv[0] << " [filename] [part] {[expected result]}" << std::endl;
    return 1;
	}
	using namespace aoc::assignment;

  std::string filename = argv[1];
  int part = std::stoi(argv[2]);
  int test_value = (argc > 3) ? std::stoi(argv[3]) : -1;

	auto line_pair = [](std::string & line){
	  // Line is of the form ###-####,##-#####
	  auto comma = line.find(",");
		auto elf0 = Sections<int>(line.substr(0, comma));
		auto elf1 = Sections<int>(line.substr(comma+1, line.size()));
	  return std::make_pair(elf0, elf1);
	};

  auto first_parser = [&](std::string & line){
	  // Line is of the form ###-####,##-#####
	  auto comma = line.find(",");
		auto elf0 = Sections<int>(line.substr(0, comma));
		auto elf1 = Sections<int>(line.substr(comma+1, line.size()));

		return duplicated_effort(elf0, elf1) ? 1 : 0;
  };

	auto second_parser = [&](std::string & line){
		return overlaps(line_pair(line)) ? 1 : 0;
	};

	int count;
	if (part != 2){
		auto scores = aoc::read_vector_from_lines<int>(filename, first_parser);
		count = std::reduce(scores.begin(), scores.end());
	} else {
	  auto scores = aoc::read_vector_from_lines<int>(filename, second_parser);
		count = std::reduce(scores.begin(), scores.end());
	}

  std::cout << ((test_value > 0) ? "Test " : "Part ") << part << " count is " << count << "\n";

	return (test_value > 0) ? (test_value == count) ? 0 : 1 : 0;

}
