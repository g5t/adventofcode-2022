#ifndef AOC_ROPE_H
#define AOC_ROPE_H
#include <set>
#include <math.h>

namespace aoc {

enum class points {left, right, up, down};

template<class T, class F=std::common_type<T,double>>
class Vec2D{
public:
	using dir_t = std::pair<T, T>;
private:
	dir_t dir_={0,0};

public:
	Vec2D(T x, T y): dir_{x, y} {}
	Vec2D(points p, T l) {
		switch(p){
		case points::left: dir_={-l, 0}; break;
		case points::right: dir_={l, 0}; break;
		case points::up: dir_={0, l}; break;
		case points::down: dir_={0, -1}; break;
		}
	}

	T x() const {return dir_.first;}
	T y() const {return dir_.second;}
	dir_t dir() const {return dir_;}

	T dot(const Vec2D & v) const {return dir_.first * v.x() + dir_.second * v.y();}
	F length() const {return static_cast<F>(sqrt(this->dot(*this)));}
	Vec2D operator+(const Vec2D &v) const {return {dir_.first + v.x(), dir_.second + v.y()};}
	Vec2D operator-(const Vec2D &v) const {return {dir_.first - v.x(), dir_.second - v.y()};}
	Vec2D & operator+=(const Vec2D &v) {dir_.first += v.x(); dir_.second += v.y(); return *this;}
	Vec2D & operator-=(const Vec2D &v) {dir_.first -= v.x(); dir_.second -= v.y(); return *this;}

	Vec2D clamp(T min, T max) const {
		auto c = [min, max](T v){return v < min ? min : v > max ? max : v;};
		return {c(x()), c(y())};
	}

	friend std::ostream & operator<<(std::ostream & os, const Vec2D & v) {
		os << "(" << v.x() << "," << v.y() << ")";
		return os;
	}
};

class RopePart;

class RopePart {
public:
	using ind_t = int;
	using pos_t = Vec2D<ind_t, double>;
private:
	pos_t pos_=pos_t(0, 0);
	std::optional<std::weak_ptr<RopePart>> tail_=std::nullopt;
public:
	RopePart() = default;
	explicit RopePart(pos_t v): pos_(v) {};
	explicit RopePart(std::shared_ptr<RopePart> rp): pos_(pos_t({0,0})), tail_(rp) {};

	pos_t position() const {return pos_;}

	void move(const pos_t & dir) {
		pos_ += dir;
		if (tail_.has_value()) tail_.value().lock()->update(pos_);
	}
	void update(const pos_t & head) {
		// cast to an integer as a cheap floor?
		if (static_cast<int>((head - pos_).length()) > 1){
			// Decide which way we move based on how things are *after* the step:
			auto d = (head - pos_).clamp(-1,1);
			move(d);
		}
	}

	friend std::ostream & operator<<(std::ostream & os, const RopePart & p){
		os << p.pos_;
		return os;
	}
};

class Rope {
public:
	using ind_t = RopePart::ind_t;
	using pos_t = RopePart::pos_t;
	using log_t = std::set<pos_t::dir_t>;
private:
  std::vector<std::shared_ptr<RopePart>> s_;
	log_t hist_={{0,0}};

public:
	explicit Rope(size_t Len) {
		s_.clear();
		s_.push_back(std::make_shared<RopePart>(pos_t({0,0})));
		for (size_t i=0; i<Len; ++i){
			s_.push_back(std::make_shared<RopePart>(s_.back()));
		}
	}

	void move(points dir, ind_t steps){
		auto m = pos_t(dir, 1);
		for (ind_t i=0; i<steps; ++i) {
			// tell the head to move
			s_.back()->move(m);
			// record the tail position
			hist_.insert(s_[0]->position().dir());
		}
	}

	size_t visit_count() const {
		return hist_.size();
	}

	friend std::ostream & operator<<(std::ostream & os, const Rope& r) {
		for (const auto & p: r.s_) os << *p << " ";
		os << "\n";
		return os;
	}
};
} // namespace
#endif
