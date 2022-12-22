#include "monkey_map.h"

using namespace aoc::monkey;
using namespace aoc::monkey::map;

  void aoc::monkey::map::Next::set(aoc::monkey::heading h, aoc::monkey::ind_t v) {
    switch (h) {
    case heading::L: next_[0] = v; break;
    case heading::R: next_[1] = v; break;
    case heading::U: next_[2] = v; break;
    case heading::D: next_[3] = v; break;
    }
  }
  bool aoc::monkey::map::Next::has(aoc::monkey::heading h) const {
    switch(h) {
    case heading::L: return next_[0].has_value();
    case heading::R: return next_[1].has_value();
    case heading::U: return next_[2].has_value();
    case heading::D: return next_[3].has_value();
    }
  }
  bool aoc::monkey::map::Next::any() const {
    return next_[0].has_value() || next_[1].has_value() || next_[2].has_value() || next_[3].has_value();
  }
  aoc::monkey::ind_t aoc::monkey::map::Next::get(heading h) const {
    switch(h) {
    case heading::L: return next_[0].value();
    case heading::R: return next_[1].value();
    case heading::U: return next_[2].value();
    case heading::D: return next_[3].value();
    }
  }


  aoc::monkey::map::Map::Map(const std::vector<std::string>& graph) {
    auto rows = graph.size();
    auto cols = graph[0].size();
    for (const auto & row: graph) if (row.size() > cols) cols = row.size();

    map_.resize(rows);
    for (auto & row: map_) row.resize(cols);
    // set left and right connections
    for (size_t r=0; r<rows; ++r){
      const auto & row = graph[r];
      auto first = cols+1;
      auto last = cols+1;
      for (size_t c=0; c<row.size(); ++c) if (first > cols && '.' == row[c]) first=c;
      for (size_t c=row.size(); c-->0; ) if (last > cols && '.' == row[c]) last=c;

      bool pblock = first > 0 && '#' == row[first-1];
      bool nblock = last < row.size()-1 && '#' == row[last+1];
      auto fprev = last;
      auto lnext = first;
      if (pblock || nblock){
        fprev = first - 1;
        lnext = last + 1;
      }
      if (pblock && !nblock) lnext = fprev;
      if (nblock && !pblock) fprev = lnext;

      if ('.' == row[fprev]) map_[r][first].set(heading::L, fprev);
      if ('.' == row[first+1]) map_[r][first].set(heading::R, first+1);
      if ('.' == row[last-1]) map_[r][last].set(heading::L, last-1);
      if ('.' == row[lnext]) map_[r][last].set(heading::R, lnext);
      for (size_t c=first+1; c<last; ++c){
        if ('#' == row[c]) continue;
        if ('.' == row[c-1]) map_[r][c].set(heading::L, c-1);
        if ('.' == row[c+1]) map_[r][c].set(heading::R, c+1);
      }
    }
    // set up and down connections
    for (size_t c=0; c<cols; ++c){
      std::vector<char> col; col.reserve(cols);
      for (const auto & row: graph) col.push_back(row.size() > c ? row[c] : ' ');
      auto first = rows+1;
      auto last = rows+1;
      for (size_t r=0; r<rows; ++r) if (first > rows && '.' == col[r]) first=r;
      for (size_t r=rows; r-->0;) if (last > rows && '.' == col[r]) last=r;
      
      bool pblock = first > 0 && '#' == col[first-1];
      bool nblock = last < col.size()-1 && '#' == col[last+1];
      auto fprev = last;
      auto lnext = first;
      if (pblock || nblock){
        fprev = first - 1;
        lnext = last + 1;
      }
      if (pblock && !nblock) lnext = fprev;
      if (nblock && !pblock) fprev = lnext;

      if ('.' == col[fprev]) map_[first][c].set(heading::U, fprev);
      if ('.' == col[first+1]) map_[first][c].set(heading::D, first+1);
      if ('.' == col[last-1]) map_[last][c].set(heading::U, last-1);
      if ('.' == col[lnext]) map_[last][c].set(heading::D, lnext);
      for (size_t r=first+1; r<last; ++r){
        if ('#' == col[r]) continue;
        if ('.' == col[r-1]) map_[r][c].set(heading::U, r-1);
        if ('.' == col[r+1]) map_[r][c].set(heading::D, r+1);
      }
    }
  }

  size_t aoc::monkey::map::Map::rows() const {return map_.size();}
  size_t aoc::monkey::map::Map::cols() const {return map_[0].size();}
  const aoc::monkey::map::Map::map_t & aoc::monkey::map::Map::map() const {return map_;}

  const aoc::monkey::map::Map::next_t & aoc::monkey::map::Map::at(std::pair<ind_t, ind_t> pair) const {return map_[pair.first][pair.second];}	

  std::vector<std::string> aoc::monkey::map::Map::to_strings() const {
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


  aoc::monkey::map::Runner::Runner(const std::vector<std::string>& graph, const std::string & list): map_(graph) {
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

	const aoc::monkey::map::Runner::map_t & aoc::monkey::map::Runner::map() const {return map_;}
	const aoc::monkey::map::Runner::program_t & aoc::monkey::map::Runner::program() const {return program_;}
	const aoc::monkey::map::Runner::path_t & aoc::monkey::map::Runner::path() const {return path_;}

	void aoc::monkey::map::Runner::run() {
	  state_t state{std::make_pair(0,0), heading::R};
		// find the starting place -- the row=0 first column that points to its next neighbor:
		while (state.first.second < map_.cols() && !map_.at(state.first).any()) ++state.first.second;

		path_.emplace_back(state);
		//run the program
		for (const auto & step: program_){
		  if (std::holds_alternative<ind_t>(step)) {
			  auto n = std::get<ind_t>(step);
				ind_t i=0;
				while (i++<n && map_.at(state.first).has(state.second)){
				  if (heading::R == state.second || heading::L == state.second){
					  state.first.second = map_.at(state.first).get(state.second);
					} else {
					  state.first.first = map_.at(state.first).get(state.second);
					}
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

	ind_t aoc::monkey::map::Runner::password() {
	  run();
		const auto & state = path_.back();
		ind_t pass{1000 * (1+state.first.first) + 4 * (1+state.first.second)} ;
		switch (state.second){
		case heading::R: pass += 0; break;
		case heading::D: pass += 1; break;
		case heading::L: pass += 2; break;
		case heading::U: pass += 3; break;
		}
		return pass;
	}



