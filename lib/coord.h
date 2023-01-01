#ifndef AOC_COORD_H
#define AOC_COORD_H
#include <iostream>
#include <tuple>


namespace aoc::coord2 {
enum class direction {nw, n, ne, e, se, s, sw, w};
direction direction_from_char(char c);
char direction_to_char(direction d);

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


}
#endif
