#include <tuple>
#include "coord.h"

using namespace aoc::coord2;

aoc::coord2::direction aoc::coord2::direction_from_char(char c){
  switch(c){
	case '<': return direction::w;
	case '^': return direction::n;
	case '>': return direction::e;
  case 'v': return direction::s;
	default: throw std::runtime_error("Unknown direction for provided char");
	}
}

char aoc::coord2::direction_to_char(aoc::coord2::direction d){
  switch(d){
	case direction::w: return '<';
	case direction::n: return '^';
	case direction::e: return '>';
  case direction::s: return 'v';
	default: throw std::runtime_error("Unknown char for provided direction");
	}
}

