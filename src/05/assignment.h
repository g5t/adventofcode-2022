

#ifndef AOC_ASSIGNMENT_H
#define AOC_ASSIGNMENT_H

#include <string>

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

#endif
