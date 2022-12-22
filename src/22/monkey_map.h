#ifndef AOC_MONKEY_MAP_H
#define AOC_MONKEY_MAP_H
#include <array>
#include <vector>
#include <optional>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <variant>
#include "monkey_common.h"

namespace aoc::monkey::map {

class Next {
public:
  using next_t = std::array<std::optional<aoc::monkey::ind_t>, 4>;
private:
  next_t next_{{std::nullopt, std::nullopt, std::nullopt, std::nullopt}};
public:
  Next() = default;
  void set(aoc::monkey::heading h, aoc::monkey::ind_t v);
  bool has(aoc::monkey::heading h) const;
  bool any() const;
  aoc::monkey::ind_t get(aoc::monkey::heading h) const;
  friend std::ostream & operator<<(std::ostream & os, const Next & n){
    os << "[";
    if (n.has(aoc::monkey::heading::L)) os << std::setw(2) << n.get(aoc::monkey::heading::L); else os << "  ";
    os << ",";
    if (n.has(aoc::monkey::heading::R)) os << std::setw(2) << n.get(aoc::monkey::heading::R); else os << "  ";
    os << ",";
    if (n.has(aoc::monkey::heading::U)) os << std::setw(2) << n.get(aoc::monkey::heading::U); else os << "  ";
    os << ",";
    if (n.has(aoc::monkey::heading::D)) os << std::setw(2) << n.get(aoc::monkey::heading::D); else os << "  ";
    os << "]";
    return os;
  }
};

class Map {
public:
  using next_t = Next;
  using row_t = std::vector<next_t>;
  using map_t = std::vector<row_t>;
private:
  map_t map_;
public:
  Map(const std::vector<std::string>& graph);
  size_t rows() const;
  size_t cols() const;
  const map_t & map() const;
  const next_t & at(std::pair<aoc::monkey::ind_t, aoc::monkey::ind_t> pair) const;
  std::vector<std::string> to_strings() const;
  friend std::ostream & operator<<(std::ostream & os, const Map & map){
    os << "Monkey map: " << map.rows() << " by " << map.cols() << "\n";
    for (const auto & row: map.to_strings()) os << row << "\n";
    return os;
  }
};

class Runner{
public:
  enum class turn {left, right};
  using map_t = Map;
  using step_t = std::variant<aoc::monkey::ind_t, turn>;
  using program_t = std::vector<step_t>;
	using state_t = std::pair<std::pair<aoc::monkey::ind_t, aoc::monkey::ind_t>, aoc::monkey::heading>;
	using path_t = std::vector<state_t>;

private:
  map_t map_;
  program_t program_;
	path_t path_;

public:
  Runner(const std::vector<std::string>& graph, const std::string & list);
	const map_t & map() const;
	const program_t & program() const;
	const path_t & path() const;
	void run();
	aoc::monkey::ind_t password();

  friend std::ostream & operator<<(std::ostream & os, const Runner & run){
	  auto graph = run.map().to_strings();
		for (const auto & state: run.path()) graph[state.first.first][state.first.second] = aoc::monkey::heading_char(state.second);
		for (const auto & line: graph) std::cout << line << "\n";
//
//		for (const auto & v: run.program()){
//		  if (std::holds_alternative<aoc::monkey::ind_t>(v)){
//			  os << std::get<aoc::monkey::ind_t>(v);
//			} else {
//			  auto t = std::get<turn>(v);
//				switch(t){
//				case turn::left: os << "L";
//				case turn::right: os << "R";
//			  }
//			}
//		}
//		os << "\n";
		return os;
	}
};



}// namespace aoc::monkey::map
#endif
