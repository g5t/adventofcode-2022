#ifndef AOC_WINDS_H
#define AOC_WINDS_H
#include <vector>
#include <optional>
#include <iomanip>
#include <iostream>

#include "libaoc.h"


namespace aoc::winds {
using ind_t = int64_t;

class State {
  ind_t x_=0;
	ind_t y_=0;
	ind_t t_=0;
	ind_t w_=0;
public:
  State() = default;
	State(ind_t x, ind_t y, ind_t t, ind_t w): x_(x), y_(y), t_(t), w_(w) {}

	ind_t x() const {return x_;}
	ind_t y() const {return y_;}
	ind_t t() const {return t_;}
	ind_t w() const {return w_;}

	bool operator==(const State& p) const {return x_==p.x() && y_==p.y() && t_==p.t() && w_==p.w();}
	bool operator!=(const State& p) const {return x_!=p.x() || y_!=p.y() || t_!=p.t() || w_!=p.w();}
};

class Wind {
public:
  using pos_t = aoc::coord2::Coord<ind_t>;
	using dir_t = aoc::coord2::direction;
private:
  pos_t pos_;
	dir_t dir_;
public:
  Wind(pos_t p, dir_t d): pos_(p), dir_(d) {}
	Wind(ind_t x, ind_t y, dir_t d): pos_(x,y), dir_(d) {}

  pos_t pos() const {return pos_;}
	dir_t dir() const {return dir_;}

	std::weak_ordering operator<=>(const Wind& w) const{
	  if (auto wo = pos_ <=> w.pos(); wo != std::weak_ordering::equivalent) return wo;
		return dir_ <=> w.dir();
	}
	bool operator==(const Wind& w) const{ return (*this <=> w) == std::weak_ordering::equivalent;}
};

class Board {
public:
	using winds_t = std::vector<Wind>;

private:
	ind_t in_;
	ind_t out_;
	ind_t rows_;
	ind_t cols_;
	winds_t winds_;

public:
  Board(ind_t i, ind_t o, ind_t r, ind_t c, winds_t w): in_(i), out_(o), rows_(r), cols_(c), winds_(std::move(w)) {}
  Board(const std::vector<std::string>& lines){
	  /* A board looks like
		 *   ###.######
		 *   #....<...#
		 *   #..^.....#
		 *   ######.###
		 * where '.' are empty, # are walls, and ^>v< are wind storms
		 * */
		rows_ = static_cast<ind_t>(lines.size());
    auto cols = lines[0].size();
		for (const auto & row: lines) if (row.size() != cols) throw std::runtime_error("Bad input board shape");
		cols_ = static_cast<ind_t>(cols);

		size_t n_winds{0};
		for (const auto & line: lines) for (const auto & c: line) if ('#' != c && '.' !=c) ++n_winds;
		winds_.reserve(n_winds);

		std::cout << n_winds << " winds out of a possible " << (rows_-2)*(cols_-2) << " spaces\n";

		for (size_t i=1; i<static_cast<size_t>(rows_); ++i){
		  for (size_t j=1; j<static_cast<size_t>(cols_); ++j){
			  if ('.' != lines[i][j] && '#' != lines[i][j]) winds_.emplace_back(static_cast<ind_t>(j), rows_-1-static_cast<ind_t>(i), aoc::coord2::direction_from_char(lines[i][j]));
			}
		}

//		std::cout << "Winds at";
//		for (const auto & w: winds_) std::cout << " " << w.pos();
//		std::cout << "\n";

		in_ = 0;
		for (size_t i=0; i<cols; ++i) if ('.' == lines.front()[i]) {in_ = static_cast<ind_t>(i); break;}
		out_ = 0;
		for (size_t i=0; i<cols; ++i) if ('.' == lines.back()[i]) {out_ = static_cast<ind_t>(i); break;}
	}

  ind_t in() const {return in_;}
	ind_t out() const {return out_;}
	ind_t rows() const {return rows_;}
	ind_t cols() const {return cols_;}
	const winds_t & winds() const {return winds_;}

  std::weak_ordering properties_ufo(const Board & b) const {
	  if (in_ < b.in()) return std::weak_ordering::less;
	  if (out_ < b.out()) return std::weak_ordering::less;
	  if (rows_ < b.rows()) return std::weak_ordering::less;
	  if (cols_ < b.cols()) return std::weak_ordering::less;
	  if (in_ > b.in()) return std::weak_ordering::greater;
	  if (out_ > b.out()) return std::weak_ordering::greater;
	  if (rows_ > b.rows()) return std::weak_ordering::greater;
	  if (cols_ > b.cols()) return std::weak_ordering::greater;
		if (winds_.size() < b.winds().size()) return std::weak_ordering::less;
		if (winds_.size() > b.winds().size()) return std::weak_ordering::greater;
		return std::weak_ordering::equivalent;
	}
	
	std::weak_ordering operator<=>(const Board & b) const {
	  if (auto pufo = properties_ufo(b); pufo != std::weak_ordering::equivalent) return pufo;
		const auto & bwinds = b.winds();
		bool all{true};
		for (const auto & w: bwinds) if (std::find(winds_.begin(), winds_.end(), w) == winds_.end()) all=false;
		if (!all) return std::weak_ordering::less;
		for (const auto & w: winds_) if (std::find(bwinds.begin(), bwinds.end(), w) == bwinds.end()) all=false;
		if (!all) return std::weak_ordering::greater;
		// all winds in both, matching shapes and in/out, so they are equivalent
		return std::weak_ordering::equivalent;
	}
	bool operator==(const Board & b) const {return (*this <=> b) == std::weak_ordering::equivalent;}
	bool operator!=(const Board & b) const {
	  if (properties_ufo(b) != std::weak_ordering::equivalent) return true;
		const auto & bwinds = b.winds();
		for (const auto & w: bwinds) if (std::find(winds_.begin(), winds_.end(), w) == winds_.end()) return true;
		for (const auto & w: winds_) if (std::find(bwinds.begin(), bwinds.end(), w) == bwinds.end()) return true;
		return false;
	}

	std::pair<size_t, size_t> indexes(const Wind::pos_t & at) const {
	  return {static_cast<size_t>(rows_-1-at.y()), static_cast<size_t>(at.x())};
	}

  std::vector<std::vector<char>> board() const {
	  std::vector<std::vector<char>> out;
		out.reserve(static_cast<size_t>(rows_));
		for (ind_t r=0; r<rows_; ++r){
		  std::vector<char> row;
			row.reserve(static_cast<size_t>(cols_));
			for (ind_t c=0; c<cols_; ++c) row.push_back('.');
			out.push_back(row);
		}
		for (ind_t c=0; c<cols_; ++c) out.front()[static_cast<size_t>(c)] = out.back()[static_cast<size_t>(c)] = '#';
		for (ind_t r=0; r<rows_; ++r) out[static_cast<size_t>(r)][0] = out[static_cast<size_t>(r)][static_cast<size_t>(cols_-1)] = '#';
		out.front()[static_cast<size_t>(in_)] = '.';
		out.back()[static_cast<size_t>(out_)] = '.';

		for (const auto & wind: winds_) 
		  out[static_cast<size_t>(rows_-1-wind.pos().y())][static_cast<size_t>(wind.pos().x())] = aoc::coord2::direction_to_char(wind.dir());

		return out;
	}

	std::vector<std::vector<char>> board_options(const Wind::pos_t & at) const {
	  auto b = board();
		for (const auto f: from(at)){
		  auto p = indexes(f);
			b[p.first][p.second] = 'o';
		}
		auto a = indexes(at);
		b[a.first][a.second] = (b[a.first][a.second] == 'o' ? 'S' : 'X');
		return b;
	}

	Board next() const {
	  winds_t new_winds;
		new_winds.reserve(winds_.size());
		for (const auto & wind: winds_) {
		  auto next_pos = wind.pos() + wind.dir();
			auto next_x = next_pos.x();
			auto next_y = next_pos.y();
			if (next_x < 1) next_x = cols_-2;
			if (next_x > cols_-2) next_x = 1;
			if (next_y < 1) next_y = rows_-2;
			if (next_y > rows_-2) next_y = 1;
			next_pos.set(std::make_pair(next_x, next_y));

			new_winds.emplace_back(next_pos, wind.dir());
		}
		return {in_, out_, rows_, cols_, std::move(new_winds)};
	}

	bool offboard(const Wind::pos_t & at) const {
	  auto x = at.x();
		auto y = at.y();
		if (x <= 0 || x >= cols_ - 1) return true;
		if (y < 0) return true;
		if (y == 0 && x != out_) return true;
		if (y == rows_ - 1 && x != in_) return true;
		if (y > rows_ -1) return true;
		return false;
	}

	bool free(const Wind::pos_t & at) const {
	  if (offboard(at)) return false;
//	  std::cout << "Free check: at = " << at << " winds = [";
//		for (const auto & w: winds_) std::cout << " " << w.pos();
//		std::cout << " ]" << "\n";
		for (const auto & w: winds_) if (w.pos() == at) return false;
		return true;
	}

	std::vector<Wind::pos_t> from(const Wind::pos_t & at) const {
	  std::vector<Wind::pos_t> options{at+aoc::coord2::direction::n, at+aoc::coord2::direction::e, at+aoc::coord2::direction::s, at+aoc::coord2::direction::w, at};
		std::vector<Wind::pos_t> open;
		for (const auto & opt: options) if (free(opt)) open.push_back(opt);
		return open;
	}

  friend std::ostream & operator<<(std::ostream & os, const Board & b) {
	  for (const auto & line: b.board()){
		  for (const auto & c: line) os << c;
			os << "\n";
		}
		return os;
	}
	std::string board_options_string(const Wind::pos_t & at) const {
	  std::stringstream ss;
		auto b = board();
		auto o = board_options(at);
		for (size_t i=0; i < b.size(); ++i){
		  for (const auto & c: b[i]) ss << c;
			ss << "  ";
			for (const auto & c: o[i]) ss << c;
			ss << "\n";
		}
//		for (const auto & line: board_options(at)){
//		  for (const auto & c: line) ss << c;
//			ss << "\n";
//		}
		return ss.str();
	}
};

class Walker {
public:
//  using weather_t = aoc::roll<Board>;
	using weather_t = std::vector<Board>;

private:
  weather_t weather_;
	Wind::pos_t start_;
	Wind::pos_t end_;

public:
  Walker(const std::vector<std::string> & lines): start_(std::make_pair(0,0)), end_(std::make_pair(0,0)) {
		std::vector<Board> w;
	  w.reserve(1);
		w.emplace_back(lines);
		//the weather pattern will repeat every rows_*cols_ steps (if not sooner)
		auto max_size = static_cast<size_t>((w.front().rows()-2) * (w.front().cols()-2));

		std::cout << "Expected no more than " << max_size << " unique wind patterns\n";

		while (w.size() < max_size){
		  auto next = w.back().next();
			//if (std::find(w.begin(), w.end(), next) != w.end()) break;
			w.push_back(next);
			std::cout << "\r" << w.size() << " wind patterns identified" << std::flush;
		}
		std::cout << "\n";

		// look for the first repeated pattern
		auto ptr = w.begin()+1;
		for (; ptr != w.end() && *ptr != w.front(); ++ptr);
		if (ptr == w.end()) std::cout << "No repeated pattern found! Is this an error?\n";
		w.erase(ptr, w.end());

		weather_ = weather_t(std::move(w));

		start_ = Wind::pos_t(weather_.front().in(), weather_.front().rows()-1);
		end_ = Wind::pos_t(weather_.front().out(), 0);
	}

	const weather_t & weather() const {return weather_;}

  ind_t traverse(const Wind::pos_t from, const Wind::pos_t to, ind_t elapsed) const {
		std::vector<State> stack{{from.x(), from.y(), elapsed, 0}};
		std::vector<State> solutions;
		std::optional<ind_t> shortest{std::nullopt};
	  while (!stack.empty()) {
		  // pop off the front element
			auto val = stack.front();
			stack.erase(stack.begin());
			Wind::pos_t at(val.x(), val.y());
			// stop looking if we're at the end
			if (at == to){
			  solutions.push_back(val);
				if (!shortest.has_value() || val.t() < shortest.value()) shortest = val.t();
				continue;
			}
			// of if we're already longer in time than the best-found solution
			if (shortest.has_value() && val.t() >= shortest.value()) continue;

			std::cout << "\rstack: " << std::setw(4) << stack.size() << " elapsed=" << std::setw(4) << val.t() << " wait=" << std::setw(4) << val.w() << " at=" << at << std::flush;

      // get the options where we can go from here
			auto index = (static_cast<size_t>(val.t()) + 1) % weather_.size();
			auto next = weather_[index];
			auto options = next.from(at);
			// push new elements onto the stack
			for (const auto & opt: options) {
			  ind_t time{val.t()+1}, wait{opt != at ? 0 : val.w() + 1};
				if (time > 10000 || static_cast<size_t>(wait) >= weather_.size()) continue;
				State state(opt.x(), opt.y(), time, wait);
				if (std::find(stack.begin(), stack.end(), state) == stack.end()) stack.push_back(state);
			}
		}

		std::cout << "\n";

		return shortest.has_value() ? shortest.value() : -1;
  }

	ind_t walk() const {
		return traverse(start_, end_, 0);
	}

	ind_t triple() const {
	  auto first = traverse(start_, end_, 0);
		auto second = traverse(end_, start_, first);
		auto third = traverse(start_, end_, second);
		return third;
	}

};



} // namespace aoc::winds
#endif 
