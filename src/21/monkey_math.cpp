#include "monkey_math.h"

static std::string to_string(aoc::monkey::operation op){
  using namespace aoc::monkey;
  switch(op){
  case operation::plus: return "+";
  case operation::minus: return "-";
  case operation::times: return "*";
  case operation::divide: return "/";
  case operation::equals: return "==";
  }
}


std::ostream & operator<<(std::ostream & ss, aoc::monkey::operation op){
  ss << to_string(op);
  return ss;
}
std::stringstream & operator<<(std::stringstream & ss, aoc::monkey::operation op){
  ss << to_string(op);
  return ss;
}
