

#ifndef AOC_ASSIGNMENT_H
#define AOC_ASSIGNMENT_H

#include <vector>
#include <numeric>
#include <iostream>

namespace aoc::ranges {

template<class T>
class Range{
	T f_;
	T l_;
public:
  Range(T f, T l): f_(f), l_(l) {};

	T first() const {return f_;}
	T last() const {return l_;}

	size_t size() const { return static_cast<size_t>(l_ - f_) + 1u; }

	bool contains(const Range<T>& partner) const {
	  return (f_ <= partner.first() && l_ >= partner.last());
	}

	bool contains(T val) const {
	  return f_ <= val && val <= l_;
	}

	bool overlaps(const Range<T>& partner) const {
	  auto pf = partner.first();
		auto pl = partner.last();
	  if (f_ <= pf && pf <= l_) return true;
		if (f_ <= pl && pl <= l_) return true;
		if (pf <= f_ && f_ <= pl) return true;
		if (pf <= l_ && l_ <= pl) return true;
		return false;
	}

	friend std::ostream & operator<<(std::ostream & os, const Range<T>& r) {
	  os << "[" << r.first() << "," << r.last() << "]";
		return os;
	}
};

template<class T> Range<T> combine(Range<T> a, Range<T> b){
  T f = (a.first() < b.first()) ? a.first() : b.first();
	T l = (a.last() > b.last()) ? a.last() : b.last();
	return {f, l};
}

/* \brief A non-overlapping sorted set of ranges
 * */
template<class T>
class RangeSet{
  using set_t = std::vector<Range<T>>;
	set_t set_;
public:
  RangeSet() = default;
  template<class I> RangeSet(I reserve) { set_.reserve(static_cast<size_t>(reserve)); }

	const set_t & set() const {return set_;}

	void sort() {
	  std::sort(set_.begin(), set_.end(), [](auto & a, auto & b){return a.first() < b.first();});
	}

	void add(const Range<T> & r){
	  auto ptr = std::find_if(set_.begin(), set_.end(), [r](auto & s){return s.overlaps(r);});
		if (ptr != set_.end()){
		  *ptr = combine(*ptr, r);
			auto next = ptr + 1;
			if (next != set_.end() && (*next).overlaps(r)) {
			  *ptr = combine(*ptr, *next);
				set_.erase(next);
				sort();
			}
		} else {
		  set_.emplace_back(r);
		  sort();
		}
	}

	void emplace(T first, T last){
	  if (first <= last){
	    set_.emplace_back(first, last);
		  sort();
		}
	}

	size_t num() const {return set_.size();}

	size_t size() const {
	  if (set_.empty()) return 0u;
    return std::transform_reduce(set_.begin(), set_.end(), 0u, [](auto a, auto b){return a+b;}, [](auto & s){return s.size();});
	}

	bool contains(T v) const {
	  for (const auto & r: set_) if (r.contains(v)) return true;
		return false;
	}

	RangeSet<T> operator-(const Range<T> & r) const {
	  RangeSet<T> out(num());
		for (const auto & s: set_) if (s.overlaps(r)){
		  if (r.contains(s)){
			  continue; // this Range does not go into the output
			} else if (s.contains(r)) {
			  auto left = Range<T>(s.first(), r.first()-1);
				auto right = Range<T>(r.last()+1, s.last());
				if (left.size()) out.add(left);
				if (right.size()) out.add(right);
			} else {
			  if (s.first() <= r.first()){
				  out.emplace(r.last() + 1, s.last());
				} else {
				  out.emplace(s.first(), r.first() - 1);
				}
			}
		} else {
		  out.add(s);
		}
		return out;
	}
	
	friend std::ostream & operator<<(std::ostream & os, const RangeSet<T>& rs){
	  for (const auto & r: rs.set()) os << r << ",";
		return os;
	}
};

template<class T> RangeSet<T> common(const RangeSet<T> & a, const RangeSet<T> & b){
  RangeSet<T> out(a.size() > b.size() ? a.size() : b.size());
	for (const auto & ar : a.set()){
    for (const auto & br: b.set()) if (ar.overlaps(br)){
		  if (ar.contains(br)){
			  out.add(br);
			} else if (br.contains(ar)) {
			  out.add(ar);
			} else if (ar.first() < br.first()){
			  out.emplace(br.first(), ar.last());
			} else {
			  out.emplace(ar.first(), br.last());
			}
		}
	}
	return out;
}


} // namespace aoc::ranges

template<class T> aoc::ranges::RangeSet<T> operator-(const aoc::ranges::RangeSet<T> & a, const aoc::ranges::RangeSet<T> & b){
	const auto & aset = a.set();
	const auto & bset = b.set();
	using namespace aoc::ranges;
	
	std::vector<RangeSet<T>> tmp;
	tmp.resize(bset.size());
	size_t count{0};
	for (const auto & br : bset){
		for (const auto & ar : aset) if (ar.overlaps(br)) {
		  if (br.contains(ar)){
			  continue;
			} else if (ar.contains(br)) {
			  tmp[count].emplace(ar.first(), br.first()-1);
				tmp[count].emplace(br.last()+1, ar.last());
			} else if (ar.first() < br.first()) {
			  tmp[count].emplace(ar.first(), br.first()-1);
			} else {
			  tmp[count].emplace(br.last()+1, ar.last());
			}
		} else {
		  tmp[count].add(ar);
		}
		count++;
	}

	if (count < 2) return tmp[0];
	auto out = common(tmp[0], tmp[1]);
	for (size_t i=2; i<count; ++i) out = common(out, tmp[i]);
	return out;
}

template<class T> aoc::ranges::RangeSet<T> operator-(const aoc::ranges::Range<T> & a, const aoc::ranges::RangeSet<T> & b){
  aoc::ranges::RangeSet<T> ar(1);
	ar.add(a);
	return ar - b;
}
template<class T> aoc::ranges::RangeSet<T> operator-(const aoc::ranges::RangeSet<T> & a, const aoc::ranges::Range<T> & b){
  aoc::ranges::RangeSet<T> br(1);
	br.add(b);
	return a - br;
}

#endif
