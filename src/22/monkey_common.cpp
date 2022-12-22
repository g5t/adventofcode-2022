#include <string>
#include "monkey_common.h"

std::string aoc::monkey::heading_string(aoc::monkey::heading h){
  using namespace aoc::monkey;
  switch (h) {
  case heading::U: return "^";
  case heading::D: return "v";
  case heading::R: return ">";
  case heading::L: return "<";
  }
}
char aoc::monkey::heading_char(aoc::monkey::heading h){
  using namespace aoc::monkey;
  switch (h) {
  case heading::U: return '^';
  case heading::D: return 'v';
  case heading::R: return '>';
  case heading::L: return '<';
  }
}
