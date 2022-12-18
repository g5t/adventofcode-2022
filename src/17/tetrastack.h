#ifndef AOC_TETRASTACK_H
#define AOC_TETRASTACK_H
#include <memory>
#include <ranges>
#include <utility>
#include <vector>
#include <array>
#include <iostream>
#include <deque>
#include <optional>
#include <algorithm>
#include <iomanip>

namespace aoc::tetra {
enum class shape {horizontal, plus, backel, vertical, square};
enum class direction {down, left, right};
}
std::ostream & operator<<(std::ostream &, aoc::tetra::direction);

namespace aoc::tetra {

template<class T> class roll {
  std::vector<T> values_;
  typename std::vector<T>::const_iterator item_;

public:
  explicit roll(std::vector<T> && v): values_(std::move(v)) {
    item_ = values_.cbegin();
  }
  [[nodiscard]] T operator[](size_t index){return values_[index % values_.size()];}
  [[nodiscard]] size_t size() const {return values_.size();}
  [[nodiscard]] bool same(size_t a, size_t b){return (a % values_.size()) == (b % values_.size());}
};


class Shape {
public:
  using ind_t = int64_t;
  using coord_t = std::pair<ind_t, ind_t>;
	using coords_t = std::vector<coord_t>;
private:
  coords_t coords_;
	coord_t min_;
	coord_t max_;

public:
  Shape() {
    coords_={{0,0},{1,0},{2,0},{3,0},{4,0},{5,0},{6,0}};
    set_envelope();
  }
  explicit Shape(coords_t c): coords_(std::move(c)){
    set_envelope();
  }
  explicit Shape(shape sh, coord_t && pos) {
	  switch(sh){
		case shape::horizontal: coords_={{0,0},{1,0},{2,0},{3,0}}; break;
		case shape::plus: coords_={{0,1},{1,0},{1,1},{2,1},{1,2}}; break;
		case shape::backel: coords_={{0,0},{1,0},{2,0},{2,1},{2,2}}; break;
		case shape::vertical: coords_={{0,0},{0,1},{0,2},{0,3}}; break;
		case shape::square: coords_={{0,0},{1,0},{1,1},{0,1}}; break;
		}
		set_envelope();
    place(std::move(pos));
	}

private:
  void set_envelope() {
    std::vector<ind_t> x, y;
    std::transform(coords_.begin(), coords_.end(), std::back_inserter(x), [](const auto & c){return c.first;});
    std::transform(coords_.begin(), coords_.end(), std::back_inserter(y), [](const auto & c){return c.second;});

    min_ = {*std::min_element(x.begin(), x.end()), *std::min_element(y.begin(), y.end())};
    max_ = {*std::max_element(x.begin(), x.end()), *std::max_element(y.begin(), y.end())};
  }
public:
  [[nodiscard]] const coord_t & min() const {return min_;}
  [[nodiscard]] const coord_t & max() const {return max_;}
  [[nodiscard]] const coords_t & coords() const {return coords_;}

  [[nodiscard]] bool might_touch(const Shape & o) const {
	  const auto & omin = o.min();
		const auto & omax = o.max();
		// check if the bounding boxes are too far apart to touch:
	  if (omin.second - max_.second > 1) return false;
		if (min_.second - omax.second > 1) return false;
		if (omin.first - max_.first > 1) return false;
		if (min_.first - omax.first > 1) return false;
		return true;
	}

  [[nodiscard]] bool touches(const Shape & o) const {
	  if (!might_touch(o)) return false;
		for (const auto & ct: coords_) for (const auto & co: o.coords()) {
		  auto x = ct.first - co.first;
			auto y = ct.second - co.second;
			if (x == 0 && (y == 1 || y == -1)) return true;
			if (y == 0 && (x == 1 || x == -1)) return true;
		}
		return false;
	}

  [[nodiscard]] bool blocks(const Shape & o, const direction dir) const {
	  if (!might_touch(o)) return false;
		// check square-by-square for blocking
		switch(dir){
		case direction::down:
		for (const auto & ct: coords_) for (const auto & co: o.coords()) {
		  if (ct.first - co.first == 0 && co.second - ct.second == 1)
			  return true;
		}
		break;
		case direction::left:
		for (const auto & ct: coords_) for (const auto & co: o.coords()) {
		  if (ct.second - co.second == 0 && co.first - ct.first == 1)
			  return true;
		}
		break;
		case direction::right:
		for (const auto & ct: coords_) for (const auto & co: o.coords()) {
		  if (ct.second - co.second == 0 && ct.first - co.first == 1)
			  return true;
		}
		break;
		}
    return false;
	}

  coord_t is_offset(const Shape & o) const {
    const auto & oc = o.coords();
    if (coords_.size() != oc.size()) return {0, 0};
    auto d = o.min();
    d.first -= min_.first;
    d.second -= min_.second;
    for (const auto & c: coords_) if (std::none_of(oc.begin(), oc.end(), [c,d](const auto & z){
      return z.first == c.first + d.first && z.second == c.second + d.second;
    })) return {0, 0};
    return d;
  }

  [[nodiscard]] bool supports(const Shape & o) const {
    for (const auto & ct: coords_) for (const auto & co: o.coords()){
      if (ct.first - co.first == 0 and co.second - ct.second > 0) return true;
    }
    return false;
  }

  // move without checking for intersections
	void place(coord_t && to) {
    auto d = std::make_pair(to.first - min_.first, to.second - min_.second);
    for (auto & x: coords_) {x.first += d.first; x.second += d.second;}
    max_ = {max_.first + d.first, max_.second + d.second};
    min_ = std::move(to);
  }

	void move(const direction dir) {
	  switch(dir){
		case direction::left: place({min_.first - 1, min_.second}); break;
		case direction::right: place({min_.first + 1, min_.second}); break;
		case direction::down: place({min_.first, min_.second - 1}); break;
		}
	}

  friend std::ostream & operator<<(std::ostream & os, const Shape & sh){
    os << "Shape at (" << sh.min().first << "," << sh.min().second << ") with size ";
    os << "[" << sh.max().first - sh.min().first << "," << sh.max().second - sh.min().second << "]";
    return os;
  }
};

class Piece {
public:
  using shape_t = Shape;
  using piece_t = std::shared_ptr<shape_t>;
	using touch_t = std::vector<std::weak_ptr<shape_t>>;
private:
  piece_t piece_;
	touch_t touch_;
public:
	explicit Piece(shape sh, shape_t::coord_t && pos) {
	  piece_ = std::make_shared<shape_t>(sh, std::move(pos));
    touch_ = touch_t();
	}
  Piece(Piece && o) noexcept : piece_(o.piece()), touch_(std::move(o.move_touch())) {}
  Piece & operator=(Piece && o) noexcept {
    piece_ = o.piece();
    touch_ = std::move(o.move_touch());
    return *this;
  }

  [[nodiscard]] piece_t piece() const {return piece_;}

  [[nodiscard]] bool alone() const {return touch_.empty();}
  [[nodiscard]] touch_t touch() const {return touch_;}

	void add(piece_t neighbor) {
		touch_.emplace_back(neighbor);
	}

  friend std::ostream & operator<<(std::ostream & os, const Piece & p){
    os << *(p.piece())  << " with " << p.touch().size() << " touching pieces";
    return os;
  }
protected:
  touch_t && move_touch() {return std::move(touch_);}
};

class Stack {
public:
  using ind_t = Shape::ind_t;
  using shape_t = Piece;
	using stack_t = std::vector<shape_t>;
	using present_t = std::array<size_t,7>;

private:
  stack_t stack_;
//	ind_t height_=0;
	present_t present_={0,0,0,0,0,0,0};
  Shape bottom_;
  std::shared_ptr<Shape> top_;
  size_t max_count=0;
  roll<shape> pieces_;
  roll<direction> jets_;

public:
  Stack(size_t count, std::vector<shape> p, std::vector<direction> j): max_count(count), pieces_(std::move(p)), jets_(std::move(j)) {
  }

  void add(shape sh) {
	  stack_.emplace_back(sh, std::make_pair(2, height()+3));
    top_ = stack_.back().piece();
	}

  [[nodiscard]] shape_t::piece_t top() {return top_;}
  [[nodiscard]] shape_t::piece_t top() const {return top_;}

  [[nodiscard]] bool top_blocked(direction dir) const {
    if (stack_.empty()) return true; // the last block was removed, don't try and keep moving it
		auto piece = top();
	  switch(dir){
		case direction::left: {
      if (piece->min().first == 0) return true;
    }
    break;
		case direction::right: {
      if (piece->max().first > 5) return true;
    }
    break;
		case direction::down: {
      if (piece->min().second == 0) return true;
    }
    break;
		}
    if (bottom_.blocks(*piece, dir)) return true;
		for (auto itr=stack_.crbegin()+1; itr != stack_.crend(); ++itr)
		  if (itr->piece()->blocks(*piece, dir)) return true;
		return false;
	}

  void top_move(direction dir) {
    if (!top_blocked(dir)) top()->move(dir);
  }

  [[nodiscard]] stack_t::const_iterator find(const shape_t::piece_t& piece) const {
    auto itr = stack_.begin();
    for (; itr != stack_.end(); ++itr){
      if (itr->piece() == piece) return itr;
    }
    return itr;
  }

  std::tuple<bool, std::vector<stack_t::const_iterator>> edge_search(const bool which, std::vector<stack_t::const_iterator> path, stack_t::const_iterator next) {
    path.push_back(next);
    if (which) {
      // looking for the left edge
      if (next->piece()->min().first == 0) return {true, path};
    } else {
      // looking for the right edge
      if (next->piece()->max().first > 5) return {true, path};
    }

    if (next->alone()) return {false, path};
    std::vector<std::vector<stack_t::const_iterator>> options;
    for (const auto & n: next->touch()) {
      auto sn = n.lock();
      auto shape = find(sn);
      if (std::find(path.begin(), path.end(), shape) == path.end()){
        auto [finished, sub_path] = edge_search(which, path, shape);
        if (finished) options.push_back(sub_path);
      }
    }
    // only circular paths or no path to the edge
    if (options.empty()) return {false, path};
    // pick the best option -- the one with the largest minimum height
    auto lowest_height = [](const auto & v){
      std::vector<Shape::ind_t> height;
      std::transform(v.begin(), v.end(), std::back_inserter(height), [](const auto & p){return p->piece()->min().second;});
      return *std::min_element(height.begin(), height.end());
    };
    std::partial_sort(options.begin(), options.begin()+1, options.end(), [&](const auto & a, const auto & b){return lowest_height(a) > lowest_height(b);});
    return {true, options.front()};
  }

  void update_bottom(const std::vector<stack_t::const_iterator> & a, const std::vector<stack_t::const_iterator> & b){
    Shape::coords_t coords;
    auto insert_coord = [&](const auto & v){
      for (const auto & c: v->piece()->coords()) coords.emplace_back(c.first, c.second);
    };
    for (const auto & q: a) insert_coord(q);
    for (const auto & q: b) insert_coord(q);
    std::sort(coords.begin(), coords.end());
    coords.erase(std::unique(coords.begin(), coords.end()), coords.end());

    bottom_ = Shape(coords);
  }

  void add_stack_to_bottom() {
    auto coords = bottom_.coords();
    for (const auto & s: stack_) for (const auto & c: s.piece()->coords()) coords.push_back(c);
    std::sort(coords.begin(), coords.end());
    coords.erase(std::unique(coords.begin(), coords.end()), coords.end());
    stack_.clear();
    bottom_ = Shape(coords);
    present_={0,0,0,0,0,0,0};
  }

  [[nodiscard]] bool bottom_removes(const shape_t & p) const {
//    return bottom_.overlaps_or_is_above(*(p.piece()));
     return !bottom_.supports(*(p.piece()));
  }

	bool unreachable_search() {
	  // called if the last-added piece produce a blocking line across the whole shaft
		/*
		 * We know that line *must* include the newest piece ... search through touching-piece lists until
		 * we hit both sides
		 */
    auto [left_hit, left] = edge_search(true, std::vector<stack_t::const_iterator>(), stack_.end()-1);
    auto [right_hit, right] = edge_search(false, std::vector<stack_t::const_iterator>(), stack_.end()-1);

    if (!left_hit || !right_hit) return false;

    update_bottom(left, right);

    auto first_bad = std::remove_if(stack_.begin(), stack_.end(), [&](const auto & s){return bottom_removes(s);});
    stack_.erase(first_bad, stack_.end());

    // move all remaining blocks into the bottom shape, and clear the stack
    add_stack_to_bottom();
    return true;
	}

  [[nodiscard]] bool spanned() {
		auto piece = top();
	  for (const auto & p: piece->coords()) present_[static_cast<size_t>(p.first)] += 1;
		return std::all_of(present_.begin(), present_.end(), [](auto & x){return x>0;});
	}
	  
	void find_touching() {
		for (auto itr=stack_.rbegin()+1; itr!=stack_.rend(); ++itr){
      if (itr->piece()->touches(*top())){
        stack_.back().add(itr->piece());
        itr->add(top());
      }
    }
	}

	ind_t run(size_t count) {
    size_t k=0;
		for (size_t i=0; i<count && i<max_count; ++i){
	    // A piece appears
		  add(pieces_[i]);
      top_move(jets_[k++]);

			while (!top_blocked(direction::down)) {
        top_move(direction::down);
        top_move(jets_[k++]);
			}
      find_touching();
		}
    return height() - 1;
	}

  std::tuple<bool, size_t, size_t, Shape> run_until(size_t start, size_t last, size_t k){
    for (size_t i=start; i<last && i<max_count; ++i){
      add(pieces_[i]);
      top_move(jets_[k++]);
      while (!top_blocked(direction::down)) {
        top_move(direction::down);
        top_move(jets_[k++]);
      }
      find_touching();
      if (spanned() && unreachable_search()) {
        if (stack_.empty()) return std::make_tuple(true, i + 1, k, bottom_);
      }
    }
    return std::make_tuple(false, std::min(last, max_count), k, bottom_);
  }

  ind_t pattern_run(size_t chunk) {
    std::vector<std::tuple<size_t, size_t, Shape>> pairs;
    size_t i{0}, k{0}, rep_i{0};
    size_t first_i{0}, second_i{0}, first_j{0}, second_j{0};
    Shape::ind_t rep_height{0};
    bool repeat_found{false};
    while (!repeat_found && i < max_count){
      auto [broke, new_i, new_k, last] = run_until(i, i + chunk, k);
      if (broke) pairs.emplace_back(new_i-1, new_k-1, last);
      if (pairs.size() > 2){
        for (size_t ii=0; ii<pairs.size()-1; ++ii){ auto ii_s = std::get<Shape>(pairs[ii]);
          for (size_t jj=ii+1; jj<pairs.size(); ++jj){ auto jj_s = std::get<Shape>(pairs[jj]);
            first_i = std::get<0>(pairs[ii]);
            first_j = std::get<1>(pairs[ii]);
            second_i = std::get<0>(pairs[jj]);
            second_j = std::get<1>(pairs[jj]);
            if (pieces_.same(first_i, second_i) && jets_.same(first_j, second_j))
            if (auto d = ii_s.is_offset(jj_s); d.second != 0){
              rep_height = d.second;
              rep_i = std::get<0>(pairs[jj]) - std::get<0>(pairs[ii]);
              repeat_found = true;
            }
          }
        }
      }
      k = new_k;
      i = new_i;
    }
    auto remaining = max_count - i;

    if (remaining) {
      auto repetitions = static_cast<Shape::ind_t>(remaining / rep_i);
      auto repeated_height = repetitions * rep_height;

      auto ru = i + (remaining % rep_i);

      while (i < ru) {
        auto [broke, new_i, new_k, last] = run_until(i, ru, k);
        i = new_i;
        k = new_k;
      }

      return repeated_height + height() - 1;
    }

    return height() - 1;
  }

	[[nodiscard]] const stack_t & stack() const {return stack_;}
  [[nodiscard]] ind_t stack_height() const {
    std::vector<ind_t> y; y.reserve(stack_.size());
    std::transform(stack_.begin(), stack_.end(), std::back_inserter(y), [](const auto & x){return x.piece()->max().second;});
    return *std::max_element(y.begin(), y.end());
  }
  [[nodiscard]] ind_t height() const {
    return 1 + (stack_.empty() ? bottom_.max().second : std::max(bottom_.max().second, stack_height()));
  }
  [[nodiscard]] ind_t back_height() const {
    if (stack_.empty()) return 0;
	  auto piece = top();
	  return 1 + piece->max().second;
	}
  [[nodiscard]] size_t plot_height() const {return static_cast<size_t>(height() > back_height() ? height() : back_height());}

  [[nodiscard]] const Shape & bottom() const {return bottom_;}
	
	friend std::ostream& operator<<(std::ostream & os, const Stack & stack){
    const auto & bottom = stack.bottom();
    auto lowest = static_cast<size_t>(bottom.min().second);
    auto highest = std::max(static_cast<size_t>(1 + bottom.max().second), stack.plot_height());
	  std::vector<std::array<char, 9>> shaft(highest - lowest, {'|','.','.','.','.','.','.','.','|'});
		for (const auto & sh: stack.stack()){
			for (auto & sc: sh.piece()->coords()) shaft[static_cast<size_t>(sc.second)-lowest][static_cast<size_t>(1+sc.first)] = '#';
		}
		if (!stack.top_blocked(direction::down)) {
		  auto piece = stack.top();
		  for (auto & bc: piece->coords()) shaft[static_cast<size_t>(bc.second)-lowest][static_cast<size_t>(1+bc.first)] = '@';
		}
    for (const auto & c: bottom.coords()) shaft[static_cast<size_t>(c.second)-lowest][static_cast<size_t>(1+c.first)] = '%';

    for (auto itr = shaft.crbegin(); itr != shaft.crend(); ++itr){
      for (const auto & c: *itr) os << c;
			os << "\n";
		}

		return os;
	}

};

} // namespace aoc::tetra
#endif
