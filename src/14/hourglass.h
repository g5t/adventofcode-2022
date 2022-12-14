#ifndef AOC_HOURGLASS_H
#define AOC_HOURGLASS_H
#include <cmath>
#include <vector>
#include <cassert>
#include <string>
#include <iostream>

#include "libaoc.h"

namespace aoc::sand{

enum class Material {air, stone, sand};

std::map<Material, aoc::bitmap::Color> material_color_map();

template<class T>
class Vec2d {
public:
  using dir_t = std::pair<T,T>;
private:
  dir_t dir_;
public:
  Vec2d(dir_t d): dir_(d) {}
	template<class R>
	Vec2d(std::pair<R,R> d): dir_({static_cast<T>(d.first), static_cast<T>(d.second)}) {}
	Vec2d(T x, T y): dir_({x, y}) {}
	
	dir_t dir() const {return dir_;}

	template<class R, class S=std::common_type_t<T,R>>
	Vec2d<S> operator+(const Vec2d<R>& o) const {
	  auto odir = o.dir();
	  return {static_cast<S>(dir_.first) + static_cast<S>(odir.first), static_cast<S>(dir_.second) + static_cast<S>(odir.second)};
	}
	template<class R, class S=std::common_type_t<T,R>>
	Vec2d<S> operator-(const Vec2d<R>& o) const {
	  auto odir = o.dir();
	  return {static_cast<S>(dir_.first) - static_cast<S>(odir.first), static_cast<S>(dir_.second) - static_cast<S>(odir.second)};
	}
	Vec2d<T>& operator+=(const Vec2d<T>& o){
	  auto odir = o.dir();
		dir_.first += odir.first;
		dir_.second += odir.second;
		return *this;
	}
	Vec2d<T>& operator-=(const Vec2d<T>& o){
	  auto odir = o.dir();
		dir_.first -= odir.first;
		dir_.second -= odir.second;
		return *this;
	}

	template<class R, class S=std::common_type_t<T,R>>
	S dot(const Vec2d<R>& o) const {
	  auto odir = o.dir();
	  return static_cast<S>(dir_.first) * static_cast<S>(odir.first) + static_cast<S>(dir_.second) * static_cast<S>(odir.second);
	}
  T norm() const {
		return static_cast<T>(std::sqrt(static_cast<double>(dot(*this))));
	}
	double fnorm() const {
		return std::sqrt(static_cast<double>(dot(*this)));
	}
	Vec2d<T> hat() const {
	  auto n = norm();
		return {dir_.first / n, dir_.second / n};
	}
	bool operator!=(const Vec2d<T> & o) const {
	  auto odir = o.dir();
	  return dir_.first != odir.first || dir_.second != odir.second;
	}

	friend std::ostream & operator<<(std::ostream & os, const Vec2d<T>& v){
	  auto d = v.dir();
		os << d.first << "," << d.second;
		return os;
	}
};

class HourGlass {
public:
  using el_t = Material;
  using ind_t = unsigned;
  using row_t = std::vector<el_t>;
  using grid_t = std::vector<row_t>;
  using coord_t = std::pair<ind_t, ind_t>;
	using vec_t = Vec2d<int>;

private:
  grid_t grid_;
  coord_t xlim_;
  coord_t ylim_;

public:
  HourGlass(coord_t x, coord_t y, const std::vector<std::vector<coord_t>> & stones): xlim_(x), ylim_(y) {
    auto xs = (xlim_.second - xlim_.first) + 1u;
    auto ys = (ylim_.second - ylim_.first) + 1u;
  
	  // Initialize the grid to all-air
    grid_.resize(ys);
    for (auto & r: grid_){
      r.resize(xs);
      for (auto & c: r) c = Material::air;
    }
		// Then fill in the stone paths
    for (const auto & path: stones){
		  for (ind_t i=1; i<path.size(); ++i){
			  auto a = vec_t(path[i-1]);
				auto b = vec_t(path[i]);

        //std::cout << a << " -> ";

				auto h = (b-a).hat();
				for (; a != b; a+=h){
				  set(a, Material::stone);
				} 
				set(b, Material::stone);
		  }
			//std::cout << vec_t(path.back()) << "\n";
		}
  }

  const grid_t & grid() const {return grid_;}

	el_t & element(const vec_t & at) {
		assert(inbounds(at));
		auto d = at.dir();
		auto x = static_cast<ind_t>(d.first) - xlim_.first;
		auto y = static_cast<ind_t>(d.second) - ylim_.first;
		return grid_[y][x];
	}
	const el_t & element(const vec_t & at) const {
		assert(inbounds(at));
		auto d = at.dir();
		auto x = static_cast<ind_t>(d.first) - xlim_.first;
		auto y = static_cast<ind_t>(d.second) - ylim_.first;
		return grid_[y][x];
	}

	void set(vec_t & at, Material mat) {element(at) = mat;}
	bool open(const vec_t & at) const {return Material::air == element(at);}

	bool inbounds(const vec_t & at) const {
	  auto d = at.dir();
		auto x = static_cast<ind_t>(d.first);
		auto y = static_cast<ind_t>(d.second);
	  return xlim_.first <= x && x <= xlim_.second && ylim_.first <= y && y <= ylim_.second;
	}

	bool possible(const vec_t & at) const {
		return (inbounds(at) && open(at)) || !inbounds(at);
	}

	bool tick(vec_t at) {
		vec_t first(0, 1), second(-1,1), third(1,1); // y is small at the top
		bool stuck{false};
		while (!stuck && inbounds(at)){
		  if (auto next = at + first; possible(next)){
			  at = next;
			} else if (auto next = at + second; possible(next)) {
			  at = next;
			} else if (auto next = at + third; possible(next)) {
			  at = next;
			} else {
			  stuck = true;
			}
		}
		if (inbounds(at)) set(at, Material::sand);
		// std::cout << *this << "\n";
		return stuck;
	}

	int run() {
	  vec_t	start(500, static_cast<int>(ylim_.first));
		int count{0};
		while (open(start) && tick(start)) ++count;
		return count;
	}

	friend std::ostream & operator<<(std::ostream & os, const HourGlass & hg){
		const auto & grid = hg.grid();
		for (const auto & r: grid){
			for (const auto & c: r){
				switch(c){
				case Material::air: os << "."; break;
				case Material::sand: os << "o"; break;
				case Material::stone: os << "#"; break;
				}
			}
			os << "\n";
		}
		return os;
	}

  void write_image(const std::string & filename) const {
    auto mcm = material_color_map();
    grid_t flipped;
    flipped.reserve(grid_.size());
    for (auto itr = grid_.crbegin(); itr!=grid_.crend(); ++itr){
      flipped.push_back(*itr);
    }
    aoc::bitmap::write(mcm, flipped, filename);
  }
};

std::vector<std::pair<unsigned, unsigned>> path_from_string(const std::string&);
HourGlass from_strings(const std::vector<std::string>&, int part=0);

}//namespace aoc::sand
#endif
