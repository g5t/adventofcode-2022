#ifndef AOC_STEAM_H
#define AOC_STEAM_H
#include <vector>
#include <sstream>
#include <optional>
#include <memory>
#include <iomanip>

namespace aoc::steam {

class Valve {
public:
  using val_t = int;
  using flow_t = std::optional<val_t>;
  using name_t = std::string;
private:
  flow_t flow_ = std::nullopt;
  bool open_ = false;
  name_t name_="";
public:
  Valve(name_t n, val_t v): name_(n) {
    if (v > 0) flow_ = v;
  }
  name_t name() const {return name_;}
  bool operational() const { return flow_.has_value(); }
  bool can_open() const {
    return flow_.has_value() && !open_;
  }
  bool is_open() const {
    return open_;
  }
  val_t flow() const {
    return (open_ && flow_.has_value()) ? flow_.value() : 0;
  }
  val_t would_flow() const {
    return flow_.has_value() ? flow_.value() : 0;
  }
  void open() {open_=true;}
  void close() {open_=false;}

  friend std::ostream & operator<<(std::ostream & os, const Valve & v){
    os << v.name() << ":";
    if (!v.operational()) os << "X";
    else os << (v.is_open() ? "|" : "+") << v.would_flow();
    return os;
  }

  bool operator==(const Valve & other) const {return name_.compare(other.name()) == 0;}
  bool operator!=(const Valve & other) const {return name_.compare(other.name()) != 0;}

};

class Cave {
public:
  using leaf_t = std::shared_ptr<Valve>;
  using node_t = std::vector<std::weak_ptr<Valve>>;
private:
  leaf_t leaf_;
  node_t node_;
public:
  Cave(leaf_t ptr): leaf_(ptr) {}

  size_t size() const {return node_.size();}

  void add_option(leaf_t ptr) {
    node_.emplace_back(ptr);
  };

  leaf_t option(size_t index) const {return node_[index].lock();}

  const leaf_t & valve() const {return leaf_;}

  const node_t & list() const {return node_;}

  friend std::ostream & operator<<(std::ostream & os, const Cave & ca){
    os << *(ca.valve()) << " -> {" << ca.size() << ": ";
    std::stringstream ss;
    for (size_t i=0; i<ca.size(); ++i) ss << *(ca.option(i)) << ",";
    auto sss = ss.str();
    os << sss.substr(0, sss.size()-1) << "}";
    return os;
  }
  bool operator==(const Cave & other) const {return *(valve()) == *(other.valve());}
  bool operator!=(const Cave & other) const {return *(valve()) != *(other.valve());}
  bool operator==(const leaf_t & other) const {return *(valve()) == *other;}
  bool operator!=(const leaf_t & other) const {return *(valve()) != *other;}
  bool operator==(const std::weak_ptr<Valve> other) const {return *(valve()) == *(other.lock());}
  bool operator!=(const std::weak_ptr<Valve> other) const {return *(valve()) != *(other.lock());}

};

class PipeRunner {
public:
  using val_t = int;
  using list_t = std::vector<Cave>;
  using count_t = std::vector<val_t>;
  using dists_t = std::vector<std::vector<val_t>>;
private:
  list_t list_;
  count_t visits_;
  dists_t dists_;
public:
  PipeRunner() = default;
  PipeRunner(list_t && l) : list_(std::move(l)) {
    visits_ = count_t(list_.size(), 0);

    dists_.reserve(list_.size());
    for (size_t i=0; i<list_.size(); ++i){
      std::vector<val_t> row(list_.size(), 0);
      dists_.push_back(row);
    }
    for (size_t i=0; i<list_.size(); ++i){
      for (size_t j=0; j<i; ++j)
        dists_[i][j] = dists_[j][i];
      for (size_t j=i+1; j<list_.size(); ++j)
        dists_[i][j] = find_distance(i, j);
    }
  }

  size_t cave_index(Cave::leaf_t leaf) const {
    auto idx = std::distance(list_.begin(), std::find_if(list_.begin(), list_.end(), [&](const auto & a){return a == leaf;}));
    return static_cast<size_t>(idx);
  }

  val_t distance_known(Cave::leaf_t from, Cave::leaf_t to) const {
    if (from == to) return true;
    if (dists_[cave_index(from)][cave_index(to)] > 0) return true;
    return false;
  }

  val_t distance(Cave::leaf_t from, Cave::leaf_t to) const {
    return dists_[cave_index(from)][cave_index(to)];
  }

  const dists_t & distances() const {return dists_;}


  const list_t & list() const {return list_;}

  friend std::ostream & operator<<(std::ostream & os, const PipeRunner & pr){
    for (const auto & l: pr.list()) os << l << "\n";
    for (const auto & row: pr.distances()) {
      for (const auto & d: row) os << std::setw(4) << d;
      os << "\n";
    }
    return os;
  }

//  val_t optimal_plan(val_t time) {
//    val_t total_output = 0;
//    size_t current=0;
//    while (time > 0){
//      auto next = which_to_open(current, time);
//      std::cout << "Open " << list_[next] << " next\n";
//      time -= dists_[current][next]; // move
//      time -= 1; // open
//
//      std::swap(current, next);
//      auto valve = list_[current].valve();
//      valve->open();
//      if (time > 0) total_output += (time * valve->flow());
//    }
//    return total_output;
//  }

  val_t total_if(val_t time, const std::vector<size_t>& order) const {
    val_t total{0};
    size_t current = 0;
    for (const auto & next: order){
      // if we're out of time, we're done:
      if (time < dists_[current][next] + 1) return total;
      // otherwise, move and open
      time -= (1 + dists_[current][next]);
      // and add the outflow from the newly opened valve
      total += time * list_[next].valve()->would_flow();
      // plus update the current index
      current = next;
    }
    // we finished with time to spare!
    return total;
  }

  val_t optimal_plan(val_t time) const {
    auto openable = which_can_open();
    // we need to start somewhere :/
    auto best = total_if(time, openable);
    if (false) {
    // and the permutations algorithm requires we start with a sorted list
    std::sort(openable.begin(), openable.end());

    do {
      auto trial = total_if(time, openable);
      if (trial > best){
        for (auto & x: openable) std::cout << std::setw(4) << x;
        std::cout << " gives " << trial << "\n";
        best = trial;
      }
    } while (std::next_permutation(openable.begin(), openable.end()));
    } else {

      std::vector<size_t> opened;
      std::vector<val_t> options; options.reserve(openable.size());
      for (const auto & v: openable) {
        options.push_back(depth_first_search(openable, opened, 0u, v, time, 0));
      }
      best = *std::max_element(options.begin(), options.end());
    }

    return best;
  }

  val_t elephant_plan(val_t time) const {
    auto able = which_can_open();
    std::vector<size_t> opened;
    std::vector<val_t> options;
    options.reserve(able.size() * (able.size()-1));
    for (const auto & i: able) for (const auto & j: able) if (i != j){
      options.push_back(double_depth_first_search(able, opened, 0u, 0u, i, j, time, time, 0));
    }
    return *std::max_element(options.begin(), options.end());
  }

private:
  val_t depth_first_search(const std::vector<size_t> & openable, std::vector<size_t> opened, size_t last, size_t next, val_t time, val_t partial) const {
    auto time_to_open = 1 + dists_[last][next];
    if (time_to_open >= time) return partial; // nothing time to flow after opening
    // otherwise update the time and add the output
    time -= time_to_open;
    partial += time * list_[next].valve()->would_flow();
    // add this index to the opened list
    opened.push_back(next);
    // check if all are *now* opened:
    if (openable.size() == opened.size()) return partial;
    // and check the remaining unopened valves
    std::vector<val_t> options;
    options.reserve(openable.size() - opened.size());
    for (const auto & v: openable) if (std::find(opened.begin(), opened.end(), v) == opened.end()) {
      options.push_back(depth_first_search(openable, opened, next, v, time, partial));
    }
    if (last == 0) {
    std::cout << "from " << last << " at " << next << " with open valves [";
    for (const auto & v: opened) std::cout << v << ",";
    std::cout << "] has options ";
    for (const auto & v: options) std::cout << v << " ";
    std::cout << "\n";
    }
    // pick the biggest option to push back up the call stack
    return *std::max_element(options.begin(), options.end());
  }

  val_t double_depth_first_search(const std::vector<size_t> & able, std::vector<size_t> opened, size_t my_last, size_t el_last, size_t my_next, size_t el_next, val_t my_time, val_t el_time, val_t partial) const {
    auto not_enough_time = [&](val_t & time, size_t last, size_t next){
      auto to_open = 1 + dists_[last][next];
      // Signal to that it will take too long to open this valve
      if (to_open >= time) return true;
      time -= to_open;
      partial += time * list_[next].valve()->would_flow();
      // Signal that it doesn't take too long, and we've made it to the valve
      return false;
    };
    auto unopened_valves = [&](size_t skip){
      std::vector<size_t> remaining;
      remaining.reserve(able.size() - opened.size());
      for (const auto & a: able) if (a!=skip && std::find(opened.begin(), opened.end(), a) == opened.end()) {
        remaining.push_back(a);
      }
      return remaining;
    };
    auto record_opening = [&](size_t & last, size_t next){
      if (std::find(able.begin(), able.end(), next) != able.end()) opened.push_back(next);
      last = next;
    };
    // the entity with the most time takes the next step ... maybe this makes sense
    if (0 == my_time && 0 == el_time) [[unlikely]] return partial;
    if (my_time > el_time){
      // protect against trying to open an already-opened valve:
      if (std::find(opened.begin(), opened.end(), my_next) != opened.end()) return double_depth_first_search(able, opened, my_last, el_last, my_next, el_next, 0, el_time, partial);
      // check if we *can* open the valve, and update our remaining time if we can
      if (not_enough_time(my_time, my_last, my_next)) return double_depth_first_search(able, opened, my_last, el_last, my_next, el_next, 0, el_time, partial);
      record_opening(my_last, my_next);
      // make sure we don't try to open the valve that the elephant is going for:
      auto left = unopened_valves(el_next);
      // move the elephant (or stop if it is already out of time);
      if (left.empty()) return double_depth_first_search(able, opened, my_last, el_last, my_next, el_next, 0, el_time, partial);
      // from the remaining valves, pick all of them one at a time for *our* next valve
      std::vector<val_t> options;
      options.reserve(left.size());
      for (const auto & next: left) options.push_back(double_depth_first_search(able, opened, my_last, el_last, next, el_next, my_time, el_time, partial));
      return *std::max_element(options.begin(), options.end());
    } else {
      if (std::find(opened.begin(), opened.end(), el_next) != opened.end()) return double_depth_first_search(able, opened, my_last, el_last, my_next, el_next, my_time, 0, partial);
      if (not_enough_time(el_time, el_last, el_next)) return double_depth_first_search(able, opened, my_last, el_last, my_next, el_next, my_time, 0, partial);
      record_opening(el_last, el_next);
      auto left = unopened_valves(my_next);
      if (left.empty()) return double_depth_first_search(able, opened, my_last, el_last, my_next, el_next, my_time, 0, partial);
      std::vector<val_t> options;
      options.reserve(left.size());
      for (const auto & next: left) options.push_back(double_depth_first_search(able, opened, my_last, el_last, my_next, next, my_time, el_time, partial));
      return *std::max_element(options.begin(), options.end());
    }
  }

  val_t find_distance(size_t from, size_t to) const {
    // perform a breadth-first search to find the shortest distance
    // keep track of loops to avoid stepping (A)-(B)-(A)-... ?
    // or just ignore that computational overhead for now
    if (from == to) return 0;
    return find_distance(list_[from].valve(), list_[to].valve());
  }
  val_t find_distance(Cave::leaf_t start, Cave::leaf_t goal) const {
//    std::cout << "Find the distance from " << start << " to " << goal << "\n";
    std::vector<Cave::leaf_t> visited;
    return step_find_distance(goal, visited, list_[cave_index(start)], 0);
  }

  val_t step_find_distance(Cave::leaf_t goal, std::vector<Cave::leaf_t> visited, const Cave & current, val_t so_far) const {
    // we may have taken a branch that gives an infinite loop -- shortcut it once its gone beyond all reason:
    if (so_far > static_cast<val_t>(list_.size())) return static_cast<val_t>(2*list_.size());

    // shortcut when we've already found the requested distance
    if (distance_known(current.valve(), goal)) return so_far + distance(current.valve(), goal);

    visited.push_back(current.valve());

    std::vector<val_t> possible_distances;
    possible_distances.reserve(current.list().size());

    for (const auto & next: current.list()){
      if (next.lock() == goal) return 1+so_far;
      auto dist = static_cast<val_t>(3*list_.size());
      if (std::find(visited.begin(), visited.end(), next.lock()) == visited.end())
        dist = step_find_distance(goal, visited, list_[cave_index(next.lock())], 1+so_far);
      possible_distances.push_back(dist);
    }

//    std::cout << "Looking for " << *goal << " at " << *(current.valve()) << " we have options ";
//    for (const auto & pd: possible_distances) std::cout << std::setw(4) << pd;
//    std::cout << "\n";

    return *std::min_element(possible_distances.begin(), possible_distances.end());
  }

  val_t additional_output(size_t current, size_t index, val_t time_remaining) const {
    const auto valve = list_[index].valve();
    std::cout << "Consider valve " << *valve << "\n";
    if (!valve->can_open()) return 0;
    auto move_and_open_time = dists_[current][index] + 1;
    if (move_and_open_time > time_remaining) return 0;
    return (time_remaining - move_and_open_time) * valve->would_flow();
  }

  size_t which_to_open(size_t current, val_t time_remaining) const {
    std::vector<val_t> outputs;
    outputs.reserve(list_.size());
    for (size_t i=0; i<list_.size(); ++i) outputs.push_back(additional_output(current, i, time_remaining));

    std::cout << "Options for opening ";
    for (auto & o: outputs) std::cout << std::setw(6) << o;
    std::cout << "\n";

    return static_cast<size_t>(std::distance(outputs.begin(), std::max_element(outputs.begin(), outputs.end())));
  }

  std::vector<size_t> which_can_open() const {
    std::vector<size_t> can;
    can.reserve(list_.size());
    for (size_t i=0; i<list_.size(); ++i) if(list_[i].valve()->can_open()) can.push_back(i);
    return can;
  }

};

PipeRunner from_strings(const std::vector<std::string> & lines);


} // namespace aoc::steam
#endif
