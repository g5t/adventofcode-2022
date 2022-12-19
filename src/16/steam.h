#ifndef AOC_STEAM_H
#define AOC_STEAM_H
#include <utility>
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
  name_t name_;
public:
  Valve(name_t n, val_t v): name_(std::move(n)) {if (v > 0) flow_ = v;}
  [[nodiscard]] name_t name() const {return name_;}
  [[nodiscard]] bool operational() const { return flow_.has_value(); }
  [[nodiscard]] bool can_open() const {return flow_.has_value() && !open_;}
  [[nodiscard]] bool is_open() const {return open_;}
  [[nodiscard]] val_t flow() const {return (open_ && flow_.has_value()) ? flow_.value() : 0;}
  [[nodiscard]] val_t would_flow() const {return flow_.has_value() ? flow_.value() : 0;}
  void open() {open_=true;}
  void close() {open_=false;}
  friend std::ostream & operator<<(std::ostream & os, const Valve & v){
    os << v.name() << ":";
    if (!v.operational()) os << "X";
    else os << (v.is_open() ? "|" : "+") << v.would_flow();
    return os;
  }
  bool operator==(const Valve & other) const {return name_ == other.name();}
  bool operator!=(const Valve & other) const {return name_ != other.name();}

};

class Cave {
public:
  using leaf_t = std::shared_ptr<Valve>;
  using node_t = std::vector<std::weak_ptr<Valve>>;
private:
  leaf_t leaf_;
  node_t node_;
public:
  explicit Cave(leaf_t ptr): leaf_(std::move(ptr)) {}
  [[nodiscard]] size_t size() const {return node_.size();}
  void add_option(const leaf_t& ptr) {node_.emplace_back(ptr);}
  [[nodiscard]] leaf_t option(size_t index) const {return node_[index].lock();}
  [[nodiscard]] const leaf_t & valve() const {return leaf_;}
  [[nodiscard]] const node_t & list() const {return node_;}
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
  bool operator==(const std::weak_ptr<Valve>& other) const {return *(valve()) == *(other.lock());}
  bool operator!=(const std::weak_ptr<Valve>& other) const {return *(valve()) != *(other.lock());}

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
  explicit PipeRunner(list_t && l) : list_(std::move(l)) {
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
  [[nodiscard]] size_t cave_index(Cave::leaf_t leaf) const {
    auto idx = std::distance(list_.begin(), std::find_if(list_.begin(), list_.end(), [&](const auto & a){return a == leaf;}));
    return static_cast<size_t>(idx);
  }
  [[nodiscard]] val_t distance_known(const Cave::leaf_t& from, const Cave::leaf_t& to) const {
    if (from == to) return true;
    if (dists_[cave_index(from)][cave_index(to)] > 0) return true;
    return false;
  }
  [[nodiscard]] val_t distance(const Cave::leaf_t & from, const Cave::leaf_t & to) const {
    return dists_[cave_index(from)][cave_index(to)];
  }
  [[nodiscard]] const dists_t & distances() const {return dists_;}
  [[nodiscard]] const list_t & list() const {return list_;}
  friend std::ostream & operator<<(std::ostream & os, const PipeRunner & pr){
    for (const auto & l: pr.list()) os << l << "\n";
    for (const auto & row: pr.distances()) {
      for (const auto & d: row) os << std::setw(4) << d;
      os << "\n";
    }
    return os;
  }

  [[nodiscard]] val_t optimal_plan(val_t time) const {
    auto solutions = common_plan(time);
    std::vector<val_t> values;
    std::transform(solutions.begin(), solutions.end(), std::back_inserter(values), [](const auto & solution){return solution->second;});
    return *std::max_element(values.begin(), values.end());
  }

  [[nodiscard]] val_t elephant_plan(val_t time) const {
    auto solutions = common_plan(time);
    size_t no =solutions.size();
    val_t best{0};
    for (size_t a=0; a<no-1; ++a) {
      const auto & me = solutions[a];
      for (size_t b = a + 1; b < no; ++b) {
        const auto & el = solutions[b];
        if ((me->first & el->first)==0 && me->second + el->second > best) best = me->second + el->second;
      }
    }
    return best;
  }

  [[nodiscard]] std::vector<std::shared_ptr<std::pair<uint64_t, val_t>>> common_plan(val_t time) const {
    auto able = encode_which_can_open();
    std::vector<std::shared_ptr<std::pair<uint64_t, val_t>>> solutions;
    for (size_t valve=1; valve<list_.size(); ++valve){
      if (able & (1lu<<valve)){
        for (const auto & sol: all_solutions(able, 0u, 0u, valve, time, 0)){
          solutions.push_back(sol);
        }
      }
    }
    std::cout << solutions.size() << " possible solutions\n";
    return solutions;
  }
private:

  [[nodiscard]] std::vector<std::shared_ptr<std::pair<uint64_t, val_t>>> all_solutions(const uint64_t able, uint64_t open, size_t last, size_t next, val_t time, val_t partial) const {
    std::vector<std::shared_ptr<std::pair<uint64_t, val_t>>> solutions;
    auto opening_time = 1 + dists_[last][next];
    if (opening_time >= time) return solutions;
    time -= opening_time;
    partial += time * list_[next].valve()->would_flow();
		// keep track of open valves
    open |= (1lu << next);
		// store partial solutions for fun (and part two)
    solutions.push_back(std::make_shared<std::pair<uint64_t, val_t>>(open, partial));
    if (open != able) {
      for (size_t valve=1; valve<list_.size(); ++valve){
        auto key = 1lu << valve;
        if ((able & key) && !(open & key))
          for (const auto & opt: all_solutions(able, open, next, valve, time, partial))
            solutions.push_back(opt);
      }
    }
    return solutions;
  }

  [[nodiscard]] val_t find_distance(size_t from, size_t to) const {
    // perform a breadth-first search to find the shortest distance
    // keep track of loops to avoid stepping (A)-(B)-(A)-... ?
    // or just ignore that computational overhead for now
    if (from == to) return 0;
    return find_distance(list_[from].valve(), list_[to].valve());
  }
  [[nodiscard]] val_t find_distance(const Cave::leaf_t & start, const Cave::leaf_t & goal) const {
    std::vector<Cave::leaf_t> visited;
    return step_find_distance(goal, visited, list_[cave_index(start)], 0);
  }

  [[nodiscard]] val_t step_find_distance(const Cave::leaf_t & goal, std::vector<Cave::leaf_t> visited, const Cave & current, val_t so_far) const {
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

    return *std::min_element(possible_distances.begin(), possible_distances.end());
  }

  [[nodiscard]] uint64_t encode_which_can_open() const {
    uint64_t can{0u};
    for (uint64_t i=1; i<list_.size(); ++i) if(list_[i].valve()->can_open()) can |= (1lu << i);
    return can;
  }
};

PipeRunner from_strings(const std::vector<std::string> & lines);


} // namespace aoc::steam
#endif
