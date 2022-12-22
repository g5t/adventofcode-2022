#ifndef AOC_MONKEY_CUBE_H
#define AOC_MONKEY_CUBE_H
#include <array>
#include <vector>
#include <optional>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <variant>
#include "monkey_common.h"

namespace aoc::monkey::cube {

std::string flat_cube(const std::vector<std::string>& flat, aoc::monkey::ind_t edge, bool is_big);

class Coord {
public:
  using coord_t = std::pair<aoc::monkey::ind_t, aoc::monkey::ind_t>;
private:
  coord_t coord_{0,0};
public:
  Coord() = default;
	explicit Coord(coord_t c): coord_(c) {}
	Coord(aoc::monkey::ind_t x, aoc::monkey::ind_t y): coord_(x, y) {}
	aoc::monkey::ind_t first() const {return coord_.first;}
	aoc::monkey::ind_t second() const {return coord_.second;}

	friend std::ostream & operator<<(std::ostream & os, const Coord & c){
	  os << "(" << c.first() << "," << c.second() << ")";
	  return os;
	}

	Coord operator%(aoc::monkey::ind_t num) const {return {first()%num, second()%num};}
	Coord operator/(aoc::monkey::ind_t num) const {return {first()/num, second()/num};}
	Coord operator*(aoc::monkey::ind_t num) const {return {first()*num, second()*num};}
	Coord operator+(const Coord & other) const {return {first() + other.first(), second() + other.second()};}
};

class Next {
public:
  using coord_t = Coord;
  using next_t = std::array<std::optional<coord_t>, 4>;
	using face_t = std::array<aoc::monkey::heading, 4>;
private:
  next_t next_{{std::nullopt, std::nullopt, std::nullopt, std::nullopt}};
	face_t face_{{aoc::monkey::heading::L, aoc::monkey::heading::R, aoc::monkey::heading::U, aoc::monkey::heading::D}};
public:
  Next() = default;
  void set(aoc::monkey::heading h, coord_t v, aoc::monkey::heading);
  bool has(aoc::monkey::heading h) const;
  bool any() const;
  coord_t get(aoc::monkey::heading h) const;
	aoc::monkey::heading face(aoc::monkey::heading) const;
  friend std::ostream & operator<<(std::ostream & os, const Next & n){
    os << "[";
    if (n.has(aoc::monkey::heading::L)) os << n.get(aoc::monkey::heading::L);
    os << ",";
    if (n.has(aoc::monkey::heading::R)) os << n.get(aoc::monkey::heading::R);
    os << ",";
    if (n.has(aoc::monkey::heading::U)) os << n.get(aoc::monkey::heading::U);
    os << ",";
    if (n.has(aoc::monkey::heading::D)) os << n.get(aoc::monkey::heading::D);
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
  size_t edge_;
  bool is_big_;
public:
  Map(const std::vector<std::string>& graph);
  size_t rows() const;
  size_t cols() const;
  const map_t & map() const;
  const next_t & at(Coord pair) const;
  std::vector<std::string> to_strings() const;
  size_t edge() const {return edge_;}
  bool is_big() const {return is_big_;}
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
	using state_t = std::pair<Coord, aoc::monkey::heading>;
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
		for (const auto & state: run.path()) graph[state.first.first()][state.first.second()] = aoc::monkey::heading_char(state.second);
		auto last = run.path().back();
		graph[last.first.first()][last.first.second()] = 'X';

		os << flat_cube(graph, run.map().edge(), run.map().is_big());
		
//		for (const auto & line: graph) std::cout << line << "\n";

//		for (const auto & v: run.program()){
//		  if (std::holds_alternative<aoc::monkey::ind_t>(v)){
//			  os << std::get<aoc::monkey::ind_t>(v);
//			} else {
//			  auto t = std::get<turn>(v);
//				switch(t){
//				case turn::left: os << "L"; break;
//				case turn::right: os << "R";break;
//			  }
//				os << " ";
//			}
//		}
//		os << "\n";
		return os;
	}
};

}// namespace aoc::monkey::map
#endif
