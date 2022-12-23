#ifndef AOC_GROVE_H
#define AOC_GROVE_H
#include <optional>
#include <vector>
#include <map>
#include <array>

namespace aoc::grove {

enum class direction {nw, n, ne, e, se, s, sw, w};

template<class T>
class Coord {
  T x_=0;
  T y_=0;
public:
  Coord(T a, T b): x_(a), y_(b) {}
	Coord(std::pair<T,T> p): x_(p.first), y_(p.second) {}
  Coord(direction d) {
	  x_=0;
		y_=0;
    if (direction::nw == d || direction::n == d || direction::ne == d) y_ = 1;
    if (direction::sw == d || direction::s == d || direction::se == d) y_ =-1;
    if (direction::nw == d || direction::w == d || direction::sw == d) x_ =-1;
    if (direction::ne == d || direction::e == d || direction::se == d) x_ = 1;
  }
  T x() const {return x_;}
  T y() const {return y_;}
	std::pair<T,T> pair() const {return std::make_pair(x_, y_);}

	void set(std::pair<T,T> p) {
	  x_=p.first;
		y_=p.second;
	}
	void set(const Coord<T>& p){
	  x_=p.x();
		y_=p.y();
	}

  Coord operator+(const Coord & o) const {return {x_+o.x(), y_+o.y()};}
  Coord operator-(const Coord & o) const {return {x_-o.x(), y_-o.y()};}
  Coord operator+(direction d) const {return *this + Coord(d);}
  T dot(const Coord & d) const {return x_ * d.x() + y_ * d.y();}

  bool operator!=(const Coord & o) const {return x_!=o.x() || y_!=o.y();}
	bool operator==(const Coord & o) const {return x_==o.x() && y_==o.y();}
	std::weak_ordering operator<=>(const Coord & o) const {
	  if (x_ < o.x()) return std::weak_ordering::less;
		if (x_ > o.x()) return std::weak_ordering::greater;
		if (y_ < o.y()) return std::weak_ordering::less;
		if (y_ > o.y()) return std::weak_ordering::greater;
		return std::weak_ordering::equivalent;
	}

	bool close(const Coord & o) const {
		auto v = o - *this;
		if (v.x() > 1 || v.x() < -1 || v.y() > 1 || v.y() < -1) return false;
		return true;
	}
	bool blocked(const Coord & o, const Coord & d) const{
		if (!close(o)) return false;
		return (*this - o + d).dot(d) == 0;
	}

	friend std::ostream & operator<<(std::ostream & os, const Coord & c){
		os << "(" << c.x() << "," << c.y() << ")";
		return os;
	}
};


template<class T>
class Planters {
	public:
		using coord_t = Coord<T>;
		using group_t = std::vector<coord_t>;
		using choices_t = std::array<std::array<coord_t,4>,4>;
	private:
		group_t group_;
		choices_t choices_{{{{direction::n, direction::s, direction::w, direction::e}},
			{{direction::s, direction::w, direction::e, direction::n}},
			{{direction::w, direction::e, direction::n, direction::s}},
			{{direction::e, direction::n, direction::s, direction::w}},
		}};
	public:
		Planters(group_t g): group_(std::move(g)) {}
		Planters(group_t && g): group_(std::move(g)) {}

		Planters(const std::vector<std::string> & board) {
			size_t planters{0};
			for (const auto & line: board) for (const auto & c: line) if ('#'==c) ++planters;
			group_.reserve(planters);

			T y{0};
			for (auto lptr = board.crbegin(); lptr!=board.crend(); ++lptr, ++y){
				for (T x=0; static_cast<size_t>(x) < lptr->size(); ++x){
					if ('#' == (*lptr)[static_cast<size_t>(x)]) group_.emplace_back(x, y);
				}
			}
		}

		const group_t & group() const {return group_;}

		std::map<coord_t, std::optional<coord_t>> stage_one(const std::array<coord_t,4> & choices) const {
			std::map<coord_t, std::optional<coord_t>> proposals;
			for (const auto & p: group_){
				// check for anyone within our 3x3 grid:
				if (std::any_of(group_.begin(), group_.end(), [p](const auto &c){return p!=c && p.close(c);})){
					// check each choice in order to see if we *can* move that way
					coord_t dir = choices[0];
					bool ok{false};
					for (size_t i=0; i<4 && !ok; ++i){
						dir = choices[i];
						ok = !std::any_of(group_.begin(), group_.end(), [p,dir](const auto & c){return p!=c && p.blocked(c, dir);});
					}
					// propose the move, if there was one:
					if (ok){
					  auto dest = p+dir;
						if (proposals.contains(dest)){
							// someone else got here first!
							proposals.at(dest) = std::nullopt;
						} else {
//							proposals.insert({dest, p.pair()});
//              auto src = p.pair();
//							proposals.emplace(dest, std::make_optional(src));
//
//              auto result = proposals.insert({dest, std::nullopt});
//							if (!result.second) throw std::runtime_error("Failed to insert into map!");
//							result.first->second = p.pair();
//							map_at = p.pair();
   						proposals[dest] = p;
						}
					}
				}
			}
			return proposals;
		}

 //   bool stage_two(const std::map<std::pair<T,T>, std::optional<std::pair<T,T>>>& proposals){
		bool stage_two(const std::map<coord_t, std::optional<coord_t>> & proposals){
			// for each proposed destination, only unique requests are fulfilled
			bool any_moved{false};
			if (proposals.size() == 0) return any_moved;

			for (const auto & [dest, who]: proposals) if (who.has_value()) {
				// find the proposer, based on their current position:
//				auto src =  coord_t(who.value());
				auto src = who.value();
				auto ptr = std::find_if(group_.begin(), group_.end(), [src](const auto &p){return p == src;});
				if (ptr == group_.end()) throw std::runtime_error("Proposer has disappeared?");
				// move the proposer via teleportation
				ptr->set(dest);
				// keep track of the fact that someone teleported
				any_moved = true;
			}
			return any_moved;
		}

		bool round(T number) {
			auto proposals = stage_one(choices_[static_cast<size_t>(number)%4]);
			return stage_two(proposals);
		}

		void diffuse(T steps){
			for (T i=0; i<steps; ++i){
				if (!round(i)) break;
			}
		}

		T diffuse() {
			T count=0;
			while(round(count++));
			return count;
		}


		std::pair<coord_t, coord_t> bounding_box() const {
			auto min = group_.front();
			auto max = group_.back();
			T minx{min.x()}, miny{min.y()};
			T maxx{max.x()}, maxy{max.y()};
			for (const auto & p: group_){
			  if (p.x() < minx) minx = p.x();
				if (p.x() > maxx) maxx = p.x();
				if (p.y() < miny) miny = p.y();
				if (p.y() > maxy) maxy = p.y();
			}
			min = coord_t(minx, miny);
			max = coord_t(maxx, maxy);
			return {min, max};
		}

		T empty_ground() const {
			auto bbox = bounding_box();
			auto v = bbox.second - bbox.first;
			auto area = (v.x() + 1)*(v.y() + 1);
			return area - static_cast<T>(group_.size());
		}

  friend std::ostream & operator<<(std::ostream & os, const Planters & p){
    auto bbox = p.bounding_box();
    auto ylen = static_cast<size_t>(bbox.second.y() - bbox.first.y() + 1);
		auto xlen = static_cast<size_t>(bbox.second.x() - bbox.first.x() + 1);
    
		std::vector<std::vector<char>> board;
		board.resize(ylen);
		for (auto & line: board) line.resize(xlen);
		for (auto & line: board) for (auto & c: line) c='.';
		for (const auto & g: p.group()) {
			auto x = static_cast<size_t>(g.x() - bbox.first.x());
		  auto y = static_cast<size_t>(bbox.second.y() - g.y());
		  board[y][x] = '#';
		}

		for (auto & line: board){
		  for (auto & c: line) os << c;
			os << "\n";
		}

    return os;
  }
};


} // aoc::grove

#endif
