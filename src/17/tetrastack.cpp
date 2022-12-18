#include <iostream>
#include "tetrastack.h"


std::ostream & operator<<(std::ostream & os, aoc::tetra::direction d){
  using namespace aoc::tetra;
  switch(d){
  case direction::left: os << "<"; break;
  case direction::right: os << ">"; break;
  case direction::down: os << "v"; break;
  }
  return os;
}
