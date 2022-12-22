#include "monkey_cube.h"

using namespace aoc::monkey;
using namespace aoc::monkey::cube;

void aoc::monkey::cube::Next::set(aoc::monkey::heading h, aoc::monkey::cube::Next::coord_t v, aoc::monkey::heading facing) {
  size_t idx=4;
  switch (h) {
  case heading::L: idx = 0; break;
  case heading::R: idx = 1; break;
  case heading::U: idx = 2; break;
  case heading::D: idx = 3; break;
  }
	next_[idx] = v;
	face_[idx] = facing;
}
bool aoc::monkey::cube::Next::has(aoc::monkey::heading h) const {
  switch(h) {
  case heading::L: return next_[0].has_value();
  case heading::R: return next_[1].has_value();
  case heading::U: return next_[2].has_value();
  case heading::D: return next_[3].has_value();
  }
}
bool aoc::monkey::cube::Next::any() const {
  return next_[0].has_value() || next_[1].has_value() || next_[2].has_value() || next_[3].has_value();
}
aoc::monkey::cube::Next::coord_t aoc::monkey::cube::Next::get(heading h) const {
  switch(h) {
  case heading::L: return next_[0].value();
  case heading::R: return next_[1].value();
  case heading::U: return next_[2].value();
  case heading::D: return next_[3].value();
  }
}
aoc::monkey::heading aoc::monkey::cube::Next::face(heading h) const {
  switch(h) {
  case heading::L: return face_[0];
  case heading::R: return face_[1];
  case heading::U: return face_[2];
  case heading::D: return face_[3];
  }
}

std::pair<size_t, aoc::monkey::cube::Coord> small_map_to_face(aoc::monkey::ind_t len, aoc::monkey::cube::Coord coord){
  auto cube = coord/len;
	auto face = coord%len;
	if (cube.first()==0 && cube.second()>1 && cube.second()<3) return {0, face};
	if (cube.first()==1 && cube.second()<3) return {1+cube.second(), face};
	if (cube.first()==2 && cube.second()>1 && cube.second()<4) return {2+cube.second(), face};
	throw std::runtime_error("Wrong 2D mapping!");
}
aoc::monkey::cube::Coord small_face_to_map(aoc::monkey::ind_t len, std::pair<size_t, aoc::monkey::cube::Coord> pair){
  aoc::monkey::cube::Coord c;
	switch(pair.first){
	case 0: c=Coord(0,2); break;
	case 1: c=Coord(1,0); break;
	case 2: c=Coord(1,1); break;
	case 3: c=Coord(1,2); break;
	case 4: c=Coord(2,2); break;
	case 5: c=Coord(2,3); break;
	default: throw std::runtime_error("Unknown face");
	}
	return c * len + pair.second;
}
std::pair<size_t, aoc::monkey::heading> small_neighbor_face(size_t face, aoc::monkey::heading direction) {
  /* For the test input, which is 'small' and has shape
	 *         +---+
	 *         | 0 |
	 * +---+---+---+
	 * | 1 | 2 | 3 |
	 * +---+---+---+---+
	 *         | 4 | 5 |
	 *         +---+---+
	 */
  if (face == 0) {
	  switch (direction){
		case heading::L: return {2, heading::U};
		case heading::R: return {5, heading::R};
		case heading::U: return {1, heading::U};
		case heading::D: return {3, heading::U};
	  }
	}
	if (face == 1) {
	  switch (direction) {
		case heading::L: return {5, heading::D};
		case heading::R: return {2, heading::L};
		case heading::U: return {0, heading::U};
		case heading::D: return {4, heading::D};
		}
	}
  if (face == 2) {
	  switch (direction){
		case heading::L: return {1, heading::R};
		case heading::R: return {3, heading::L};
		case heading::U: return {0, heading::L};
		case heading::D: return {4, heading::L};
	  }
	}
	if (face == 3) {
	  switch (direction) {
		case heading::L: return {2, heading::R};
		case heading::R: return {5, heading::U};
		case heading::U: return {0, heading::D};
		case heading::D: return {4, heading::U};
		}
	}
  if (face == 4) {
	  switch (direction){
		case heading::L: return {2, heading::D};
		case heading::R: return {5, heading::L};
		case heading::U: return {3, heading::D};
		case heading::D: return {1, heading::D};
	  }
	}
	if (face == 5) {
	  switch (direction) {
		case heading::L: return {4, heading::R};
		case heading::R: return {0, heading::R};
		case heading::U: return {3, heading::R};
		case heading::D: return {1, heading::L};
		}
	}
	throw std::runtime_error("Unknown face");
}

std::pair<size_t, aoc::monkey::cube::Coord> big_map_to_face(aoc::monkey::ind_t len, aoc::monkey::cube::Coord coord){
  auto cube = coord/len;
	auto face = coord%len;
	if (cube.first()==0 && cube.second()>0 && cube.second()<3) return {cube.second()-1, face};
	if (cube.first()==1 && cube.second()>0 && cube.second()<2) return {cube.second()+1, face};
	if (cube.first()==2 && cube.second()<2) return {3+cube.second(), face};
	if (cube.first()==3 && cube.second()<1) return {5, face};
	throw std::runtime_error("Wrong 2D mapping!");
}
aoc::monkey::cube::Coord big_face_to_map(aoc::monkey::ind_t len, std::pair<size_t, aoc::monkey::cube::Coord> pair){
  aoc::monkey::cube::Coord c;
	switch(pair.first){
	case 0: c=Coord(0,1); break;
	case 1: c=Coord(0,2); break;
	case 2: c=Coord(1,1); break;
	case 3: c=Coord(2,0); break;
	case 4: c=Coord(2,1); break;
	case 5: c=Coord(3,0); break;
	default: throw std::runtime_error("Unknown face");
	}
	return c * len + pair.second;
}

std::pair<size_t, aoc::monkey::heading> big_neighbor_face(size_t face, aoc::monkey::heading direction){
  /* For the puzzle input, which is 'big' and has shape
	 *     +---+---+
	 *     | 0 | 1 |
	 *     +---+---+
	 *     | 2 |
	 * +---+---+
	 * | 3 | 4 |
	 * +---+---+
	 * | 5 |
	 * +---+
	 */
  if (face == 0) {
	  switch (direction){
		case heading::L: return {3, heading::L};
		case heading::R: return {1, heading::L};
		case heading::U: return {5, heading::L};
		case heading::D: return {2, heading::U};
	  }
	}
	if (face == 1) {
	  switch (direction) {
		case heading::L: return {0, heading::R};
		case heading::R: return {4, heading::R};
		case heading::U: return {5, heading::D};
		case heading::D: return {2, heading::R};
		}
	}
  if (face == 2) {
	  switch (direction){
		case heading::L: return {3, heading::U};
		case heading::R: return {1, heading::D};
		case heading::U: return {0, heading::D};
		case heading::D: return {4, heading::U};
	  }
	}
	if (face == 3) {
	  switch (direction) {
		case heading::L: return {0, heading::L};
		case heading::R: return {4, heading::L};
		case heading::U: return {2, heading::L};
		case heading::D: return {5, heading::U};
		}
	}
  if (face == 4) {
	  switch (direction){
		case heading::L: return {3, heading::R};
		case heading::R: return {1, heading::R};
		case heading::U: return {2, heading::D};
		case heading::D: return {5, heading::R};
	  }
	}
	if (face == 5) {
	  switch (direction) {
		case heading::L: return {0, heading::U};
		case heading::R: return {4, heading::D};
		case heading::U: return {3, heading::D};
		case heading::D: return {1, heading::U};
		}
	}
	throw std::runtime_error("Unknown face");
}

aoc::monkey::cube::Coord face_to_face(aoc::monkey::heading from, aoc::monkey::heading to, aoc::monkey::ind_t len, aoc::monkey::ind_t x){
  // output face {row, col} 
  auto n=len-1;
	bool is_u_or_r = heading::U == from || heading::R == from;
	switch (to){
	case heading::L: return {is_u_or_r ? x : n-x, 0};
	case heading::R: return {is_u_or_r ? n-x : x, n};
	case heading::U: return {0, is_u_or_r ? n-x : x};
	case heading::D: return {n, is_u_or_r ? x : n-x};
	}
}

aoc::monkey::heading opposite_heading(aoc::monkey::heading dir){
  switch(dir){
	case heading::L: return heading::R;
	case heading::R: return heading::L;
	case heading::U: return heading::D;
	case heading::D: return heading::U;
  }
}


aoc::monkey::cube::Map::Map(const std::vector<std::string>& graph) {
  auto rows = graph.size();
  auto cols = graph[0].size();
	
  for (const auto & row: graph) if (row.size() > cols) cols = row.size();
	// either the rows or the columns is 4 times the face edge length
	//
	auto edge = std::max(rows, cols) / 4;
	bool is_big = edge > 4;

	edge_ = edge;
	is_big_ = is_big;

	std::cout << "Map is " << rows/edge << " by " << cols/edge << " faces for a " << edge << " sized " << (is_big ? "big" : "small") << " cube\n";

  map_.resize(rows);
  for (auto & row: map_) row.resize(cols);
  // set connections
  for (size_t r=0; r<rows; ++r){
    const auto & row = graph[r];
		
		for (size_t c=0; c<row.size(); ++c) if ('.' == row[c]) {
		  // find which face this is, and our neighbors
			auto face_index = is_big ? big_map_to_face(edge, {r, c}) : small_map_to_face(edge, {r, c});
			bool on_left{face_index.second.second() == 0}, on_right{face_index.second.second() == edge-1};
			bool on_top{face_index.second.first() == 0}, on_bottom{face_index.second.first() == edge-1};
			//
			if (on_left){
			  auto neighbor_direction = is_big ? big_neighbor_face(face_index.first, heading::L) : small_neighbor_face(face_index.first, heading::L);
				auto neighbor = face_to_face(heading::L, neighbor_direction.second, edge, face_index.second.first());
				auto coord = is_big ? big_face_to_map(edge, {neighbor_direction.first, neighbor}) : small_face_to_map(edge, {neighbor_direction.first, neighbor});
				if ('.' == graph[coord.first()][coord.second()]) {
				  map_[r][c].set(heading::L, coord, opposite_heading(neighbor_direction.second));
				}
			} else {
			  if ('.' == row[c-1]) map_[r][c].set(heading::L, {r, c-1}, heading::L);
			}
			if (on_right){
			  auto neighbor_direction = is_big ? big_neighbor_face(face_index.first, heading::R) : small_neighbor_face(face_index.first, heading::R);
				auto neighbor = face_to_face(heading::R, neighbor_direction.second, edge, face_index.second.first());
				auto coord = is_big ? big_face_to_map(edge, {neighbor_direction.first, neighbor}) : small_face_to_map(edge, {neighbor_direction.first, neighbor});
				if ('.' == graph[coord.first()][coord.second()]) {
				  map_[r][c].set(heading::R, coord, opposite_heading(neighbor_direction.second));
				}
			} else {
				if ('.' == row[c+1]) map_[r][c].set(heading::R, {r, c+1}, heading::R);
			}
			if (on_top){
			  auto neighbor_direction = is_big ? big_neighbor_face(face_index.first, heading::U) : small_neighbor_face(face_index.first, heading::U);
				auto neighbor = face_to_face(heading::R, neighbor_direction.second, edge, face_index.second.second());
				auto coord = is_big ? big_face_to_map(edge, {neighbor_direction.first, neighbor}) : small_face_to_map(edge, {neighbor_direction.first, neighbor});
				if ('.' == graph[coord.first()][coord.second()]) {
				  map_[r][c].set(heading::U, coord, opposite_heading(neighbor_direction.second));
				}
			} else {
				if ('.' == graph[r-1][c]) map_[r][c].set(heading::U, {r-1, c}, heading::U);
			}
			if (on_bottom){
			  auto neighbor_direction = is_big ? big_neighbor_face(face_index.first, heading::D) : small_neighbor_face(face_index.first, heading::D);
				auto neighbor = face_to_face(heading::D, neighbor_direction.second, edge, face_index.second.second());
				auto coord = is_big ? big_face_to_map(edge, {neighbor_direction.first, neighbor}) : small_face_to_map(edge, {neighbor_direction.first, neighbor});
				if ('.' == graph[coord.first()][coord.second()]) {
				  map_[r][c].set(heading::D, coord, opposite_heading(neighbor_direction.second));
				}
			} else {
				if ('.' == graph[r+1][c]) map_[r][c].set(heading::D, {r+1, c}, heading::D);
			}
		}
	}

}

size_t aoc::monkey::cube::Map::rows() const {return map_.size();}
size_t aoc::monkey::cube::Map::cols() const {return map_[0].size();}
const aoc::monkey::cube::Map::map_t & aoc::monkey::cube::Map::map() const {return map_;}

const aoc::monkey::cube::Map::next_t & aoc::monkey::cube::Map::at(aoc::monkey::cube::Coord pair) const {return map_[pair.first()][pair.second()];}	

std::vector<std::string> aoc::monkey::cube::Map::to_strings() const {
  std::vector<std::string> graph;
  for (size_t r=0; r<rows(); ++r){
    std::stringstream ss;
    auto up = (r == 0) ? rows() - 1 : r - 1;
    auto down = (r == rows() - 1) ? 0 : r + 1;
    for (size_t c=0; c<cols(); ++c){
      auto left = (c == 0) ? cols() - 1 : c - 1;
      auto right = (c == cols() - 1) ? 0 : c + 1;
      const auto & rc = map_[r][c];
      if (!rc.any()) {
        // either '#' or ' '
        if (!map_[r][left].any() && !map_[r][right].any() && !map_[up][c].any() && !map_[down][c].any()){
          ss << " ";
        } else if (map_[r][left].has(heading::R) || map_[r][right].has(heading::L) || map_[up][c].has(heading::D) || map_[down][c].has(heading::U)){
          // if the neighboring grid has a heading towards us, we are not blocking them ...
          ss << " ";
        } else {
          ss << "#";
        }
      } else {
        // must be '.'
        ss << '.';
      }
    }
    graph.push_back(ss.str());
  }
  return graph;
}


aoc::monkey::cube::Runner::Runner(const std::vector<std::string>& graph, const std::string & list): map_(graph) {
  program_.reserve(list.size()); // absolute maximum program size
  size_t point=0;
  while (point < list.size()) {
    if ('R' == list[point]){
      program_.emplace_back(turn::right);
			++point;
    } else if ('L' == list[point]) {
      program_.emplace_back(turn::left);
			++point;
    } else {
      auto next = list.substr(point).find_first_of("LR");
      if (next == std::string::npos) next = list.size() - point;
      program_.emplace_back(static_cast<ind_t>(std::stoll(list.substr(point, next))));
      point += next;
    }
 }
}

const aoc::monkey::cube::Runner::map_t & aoc::monkey::cube::Runner::map() const {return map_;}
const aoc::monkey::cube::Runner::program_t & aoc::monkey::cube::Runner::program() const {return program_;}
const aoc::monkey::cube::Runner::path_t & aoc::monkey::cube::Runner::path() const {return path_;}

void aoc::monkey::cube::Runner::run() {
  state_t state{{0,0}, heading::R};
	// find the starting place -- the row=0 first column that points to its next neighbor:
	while (state.first.second() < map_.cols() && !map_.at(state.first).any()) state.first = state.first + Coord(0,1);

	path_.emplace_back(state);
	//run the program
	for (const auto & step: program_){
	  if (std::holds_alternative<ind_t>(step)) {
		  auto n = std::get<ind_t>(step);
			ind_t i=0;
			while (i++<n && map_.at(state.first).has(state.second)){
			  auto new_position = map_.at(state.first).get(state.second);
				auto new_heading = map_.at(state.first).face(state.second);
				//auto new_fi = map_.is_big() ? big_map_to_face(map_.edge(), new_position) : small_map_to_face(map_.edge(), new_position);
				//auto old_fi = map_.is_big() ? big_map_to_face(map_.edge(), state.first) : small_map_to_face(map_.edge(), state.first);
				//if (old_fi.first != new_fi.first) std::cout << "Steping from " << old_fi.first << old_fi.second << " to " << new_fi.first << new_fi.second<< "\n";

				state.first = new_position;
				state.second = new_heading;
				path_.emplace_back(state);
			}
		} else {
			if (turn::right == std::get<turn>(step)){
			  switch (state.second){
				case heading::R: state.second = heading::D; break;
				case heading::D: state.second = heading::L; break;
				case heading::L: state.second = heading::U; break;
				case heading::U: state.second = heading::R; break;
				}
			} else {
			  switch (state.second){
				case heading::R: state.second = heading::U; break;
				case heading::D: state.second = heading::R; break;
				case heading::L: state.second = heading::D; break;
				case heading::U: state.second = heading::L; break;
				}
			}
		}
		path_.emplace_back(state);
	}
}

ind_t aoc::monkey::cube::Runner::password() {
  run();
	const auto & state = path_.back();
	ind_t pass{1000 * (1+state.first.first()) + 4 * (1+state.first.second())} ;
	switch (state.second){
	case heading::R: pass += 0; break;
	case heading::D: pass += 1; break;
	case heading::L: pass += 2; break;
	case heading::U: pass += 3; break;
	}
	return pass;
}



std::string aoc::monkey::cube::flat_cube(const std::vector<std::string>& flat, aoc::monkey::ind_t edge, bool is_big){
  std::stringstream ss;
	auto padding = edge - 6 + 1;
	auto pre = padding/2;
	auto post = padding - pre;
	for (size_t i=0; i<6; ++i){
	  for (size_t j=0; j<pre; ++j) ss << " ";
		ss << "face " << i;
		for (size_t j=0; j<post; ++j) ss << " ";
	}
	ss << "\n";
	for (size_t i=0; i<6; ++i){
	  for (size_t j=0; j<edge; ++j) ss << "-";
		ss << " ";
	}
	ss << "\n";

	for (aoc::monkey::ind_t r=0; r<edge; ++r){
	  for (size_t i=0; i<6; ++i){
		  for (aoc::monkey::ind_t c=0; c<edge; ++c){
			  auto coord = is_big ? big_face_to_map(edge, {i, Coord(r,c)}) : small_face_to_map(edge, {i, Coord(r, c)});
				ss << flat[coord.first()][coord.second()];
			}
			ss << " ";
		}
		ss << "\n";
	}

  return ss.str();
}
