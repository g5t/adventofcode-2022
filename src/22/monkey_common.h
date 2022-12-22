#ifndef AOC_MONKEY_COMMON_H
#define AOC_MONKEY_COMMON_H
namespace aoc::monkey {
enum class heading {L, R, U, D};
std::string heading_string(heading);
char heading_char(heading);

using ind_t = size_t;
}
#endif
