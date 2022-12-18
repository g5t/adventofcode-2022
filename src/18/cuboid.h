#ifndef AOC_CUBOID_H
#define AOC_CUBOID_H
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

#include <libaoc.h>

namespace aoc::cuboid {

//template<class T> class roll {
//  std::vector<T> values_;
//  typename std::vector<T>::const_iterator item_;
//
//public:
//  explicit roll(std::vector<T> && v): values_(std::move(v)) {
//    item_ = values_.cbegin();
//  }
//  [[nodiscard]] T operator[](size_t index){return values_[index % values_.size()];}
//  [[nodiscard]] size_t size() const {return values_.size();}
//  [[nodiscard]] bool same(size_t a, size_t b){return (a % values_.size()) == (b % values_.size());}
//};


class Cuboid {
public:
  using ind_t = int64_t;
  using coord_t = std::array<ind_t, 3>;
private:
  coord_t pos_={{0,0,0}};

public:
  Cuboid() = default;
  explicit Cuboid(coord_t c): pos_(std::move(c)) {}
  explicit Cuboid(const std::string & line){
    auto vals = aoc::split<ind_t>(line);
    pos_ = {{vals[0], vals[1], vals[2]}};
  }

  [[nodiscard]] const coord_t & at() const {return pos_;}

  [[nodiscard]] bool might_touch(const Cuboid & o) const {
    const auto & oat = o.at();
    for (size_t i=0; i<3u; ++i) if (pos_[i] > oat[i] ? pos_[i] - oat[i] > 1 : oat[i] - pos_[i] > 1) return false;
    return true;
  }

  [[nodiscard]] coord_t to(const Cuboid & o) const {
    const auto & oat = o.at();
    return {{oat[0] - pos_[0], oat[1] - pos_[1], oat[2] - pos_[2]}};
  }

  [[nodiscard]] ind_t distance(const Cuboid & o) const {
    auto v = to(o);
    return std::abs(v[0]) + std::abs(v[1]) + std::abs(v[2]);
  }

  [[nodiscard]] bool touches(const Cuboid & o) const {
    if (!might_touch(o)) return false;
    return distance(o) == 1;
  }

  [[nodiscard]] bool overlaps(const Cuboid & o) const {
    if (!might_touch(o)) return false;
    return distance(o) == 0;
  }

  [[nodiscard]] bool overlaps(const coord_t & c) const {
    for (size_t i=0; i<3u; ++i) if (pos_[i] != c[i]) return false;
    return true;
  }

  friend std::ostream & operator<<(std::ostream & os, const Cuboid & sh){
    os << "Cuboid at (" << sh.at()[0] << "," << sh.at()[1] << "," << sh.at()[2] << ")";
    return os;
  }
};

class Droplet {
public:
  using shape_t = Cuboid;
  using ind_t = shape_t::ind_t;
  using coord_t = shape_t::coord_t;
  using piece_t = std::shared_ptr<shape_t>;
  using touch_t = std::vector<std::weak_ptr<shape_t>>;
private:
  piece_t piece_;
  touch_t touch_;
public:
  explicit Droplet(const coord_t & coord) {
    piece_ = std::make_shared<shape_t>(coord);
    touch_ = touch_t();
  }
  explicit Droplet(const std::string & line) {
    piece_ = std::make_shared<shape_t>(line);
    touch_ = touch_t();
  }
  Droplet(Droplet && o) noexcept : piece_(o.piece()), touch_(std::move(o.move_touch())) {}
  Droplet & operator=(Droplet && o) noexcept {
    piece_ = o.piece();
    touch_ = std::move(o.move_touch());
    return *this;
  }

  [[nodiscard]] bool overlaps(const Droplet & o) const {return piece_->overlaps(*(o.piece()));}
  [[nodiscard]] bool overlaps(const coord_t & c) const {return piece_->overlaps(c);}

  [[nodiscard]] piece_t piece() const {return piece_;}
  [[nodiscard]] bool alone() const {return touch_.empty();}
  [[nodiscard]] touch_t touch() const {return touch_;}

  [[nodiscard]] coord_t at() const {return piece_->at();}

  void add(const Droplet & neighbor) {touch_.emplace_back(neighbor.piece());}
  void add(piece_t neighbor) {touch_.emplace_back(neighbor);}
  void clear() {touch_.clear();}

  [[nodiscard]] bool touches(const Droplet & n) const {return piece_->touches(*(n.piece()));}

  ind_t exposed_surface_area() const {return ind_t(6) - static_cast<ind_t>(touch_.size());}

  friend std::ostream & operator<<(std::ostream & os, const Droplet & p){
    os << *(p.piece())  << " with " << p.touch().size() << " touching droplets";
    return os;
  }
protected:
  touch_t && move_touch() {return std::move(touch_);}
};

class Drop {
public:
  using shape_t = Droplet;
  using ind_t = shape_t::ind_t;
  using coord_t = shape_t::coord_t;
  using stack_t = std::vector<shape_t>;

private:
  stack_t stack_;

public:
  Drop(stack_t && s): stack_(std::move(s)) {}
  explicit Drop(const std::vector<coord_t> & coords){
    stack_.reserve(coords.size());
    for (const auto & coord: coords) stack_.emplace_back(coord);
    update_touching();
  }
  explicit Drop(const std::vector<std::string>& lines){
    stack_.reserve(lines.size());
    for (const auto & line: lines) stack_.emplace_back(line);
    update_touching();
  }

  [[nodiscard]] const stack_t & stack() const {return stack_;}

  void update_touching() {
    if (stack_.size() < 2) return;
    for (size_t i=0; i<stack_.size()-1; ++i){
      for (size_t j=i+1; j<stack_.size(); ++j){
        if (stack_[i].touches(stack_[j])) {
          stack_[i].add(stack_[j]);
          stack_[j].add(stack_[i]);
        }
      }
    }
  }

  void add(const coord_t & coord) {
    stack_.emplace_back(coord);
    for (size_t i=0; i<stack_.size()-1; ++i)
    if (stack_.back().touches(stack_[i])){
      stack_[i].add(stack_.back());
      stack_.back().add(stack_[i]);
    }
  }

  [[nodiscard]] bool contains(const coord_t & coord) const {
    for (const auto & s: stack_) if (s.overlaps(coord)) return true;
    return false;
  }

  
  [[nodiscard]] stack_t::const_iterator find(const shape_t::piece_t& piece) const {
    auto itr = stack_.begin();
    for (; itr != stack_.end(); ++itr){
      if (itr->piece() == piece) return itr;
    }
    return itr;
  }

  [[nodiscard]] ind_t surface_area() const {
    return std::transform_reduce(stack_.begin(), stack_.end(), 0u,
                                 [](const auto & a, const auto & b){return a+b;},
                                 [](const auto & s){return s.exposed_surface_area();}
                                );
  }

  [[nodiscard]] ind_t flood_surface_area() const {
    auto bb_min = stack_.front().at();
    auto bb_max = stack_.back().at();
    for (const auto & s: stack_) {
      const auto & x = s.at();
      if (x[0] < bb_min[0]) bb_min[0] = x[0];
      if (x[1] < bb_min[1]) bb_min[1] = x[1];
      if (x[2] < bb_min[2]) bb_min[2] = x[2];
      if (x[0] > bb_max[0]) bb_max[0] = x[0];
      if (x[1] > bb_max[1]) bb_max[1] = x[1];
      if (x[2] > bb_max[2]) bb_max[2] = x[2];
    }
    // expand to have space:
    for (size_t i=0; i<3; ++i){
      bb_min[i] -= 2;
      bb_max[i] += 2;
    }

    auto l0 = bb_max[0] - bb_min[0] + 1;
    auto l1 = bb_max[1] - bb_min[1] + 1;
    auto l2 = bb_max[2] - bb_min[2] + 1;
    auto outer_area = 2*l0*l1 + 2*l1*l2 + 2*l0*l2;
    auto border_size = static_cast<size_t>(l0 * l1 * l2 - (l0 - 2) * (l1 - 2) * (l2 - 2));

    // build the border
    std::vector<coord_t> border;
    border.reserve(border_size);

    for (auto i=bb_min[0]; i<bb_max[0]+1; ++i) for (auto j=bb_min[1]; j<bb_max[1]+1; ++j) for (auto k=bb_min[2]; k<bb_max[2]+1; ++k){
      auto oki = i == bb_min[0] || i == bb_max[0];
      auto okj = j == bb_min[1] || j == bb_max[1];
      auto okk = k == bb_min[2] || k == bb_max[2];
      if (oki || okj || okk) border.push_back({i,j,k});
    }

    if (border.size() != border_size) return 0;
      
    Drop water(border);

    flood(water, {bb_min[0]+1, bb_min[1]+1, bb_min[2]+1});

    // the inner water surface area *is* the outer drop surface area
    // and we *know* the outer water surface area:
    //
    return water.surface_area() - outer_area;
  }

  void flood(Drop & filled, const coord_t & last) const {
    const std::vector<coord_t> dirs{{1,0,0},{0,1,0},{0,0,1},{-1,0,0},{0,-1,0},{0,0,-1}};
    std::vector<coord_t> list;
    list.reserve(6);
    for (const auto & dir: dirs){
      auto next = last;
      for (size_t i=0; i<3; ++i) next[i] += dir[i];
      if (contains(next) || filled.contains(next)) continue;
      // add the point to so_far *and* a next_list, then recurse
      filled.add(next);
      list.push_back(next);
    }
    // now recurse
    for (const auto & next: list) flood(filled, next);
  }

  
  friend std::ostream& operator<<(std::ostream & os, const Drop & drop){
    const auto & s = drop.stack();
    os << "Drop comprised of " << s.size() << " droplets:\n";
    for (const auto & d: s) os << " " << d << "\n";
    return os;
  }

};

} // namespace aoc::cuboid
#endif
