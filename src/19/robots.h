#ifndef AOC_ROBOTS_H
#define AOC_ROBOTS_H
#include <utility>
#include <vector>
#include <array>
#include <algorithm>
#include <iostream>
#include <iomanip>

namespace aoc::robots {
  using i_t = unsigned;
  enum class material {none, ore, clay, obsidian, geode};
  const std::array<material, 4> MATERIALS{{material::geode, material::obsidian, material::clay, material::ore}};
}

std::ostream & operator<<(std::ostream& os, aoc::robots::material);

namespace aoc::robots{
class Blueprint {
private:
  i_t ore_ore;
  i_t clay_ore;
  i_t obsidian_ore;
  i_t obsidian_clay;
  i_t geode_ore;
  i_t geode_obsidian;
  i_t max_ore;
public:
  Blueprint(i_t v0, i_t v1, i_t v2, i_t v3, i_t v4, i_t v5):
    ore_ore(v0), clay_ore(v1), obsidian_ore(v2), obsidian_clay(v3), geode_ore(v4), geode_obsidian(v5) {
    max_ore = std::max(std::max(ore_ore, clay_ore), std::max(obsidian_ore, geode_ore));
  }

  [[nodiscard]] i_t ore(material robot) const {
    switch(robot){
    case material::ore: return ore_ore;
    case material::clay: return clay_ore;
    case material::obsidian: return obsidian_ore;
    case material::geode: return geode_ore;
    default: return 0;
    }
  }
  [[nodiscard]] i_t clay(material robot) const {
    if (material::obsidian == robot) return obsidian_clay;
    return 0;
  }
  [[nodiscard]] i_t obsidian(material robot) const {
    if (material::geode == robot) return geode_obsidian;
    return 0;
  }
  [[nodiscard]] i_t cost(material robot, material ingredient) const {
    switch (ingredient) {
      case material::ore: return ore(robot);
      case material::clay: return clay(robot);
      case material::obsidian: return obsidian(robot);
      default: return 0;
    }
  }
  [[nodiscard]] i_t max(material ingredient) const {
    switch (ingredient){
      case material::ore: return max_ore;
      case material::clay: return obsidian_clay;
      case material::obsidian: return geode_obsidian;
      default: return 0;
    }
  }
  friend std::ostream & operator<<(std::ostream & os, const Blueprint & bp){
    os << "Each ore robot costs " << bp.cost(material::ore, material::ore) << " ore. ";
    os << "Each clay robot costs " << bp.cost(material::clay, material::ore) << " ore. ";
    os << "Each obsidian robot costs "<< bp.cost(material::obsidian, material::ore) <<" ore and " << bp.cost(material::obsidian, material::clay) << " clay. ";
    os << "Each geode robot costs " << bp.cost(material::geode, material::ore)  << " ore and " << bp.cost(material::geode, material::obsidian) << " obsidian.";
    return os;
  }
  [[nodiscard]] std::weak_ordering compare(material a, material b) const{
    return (ore(a) + clay(a) + obsidian(a)) <=> (ore(b) + clay(b) + obsidian(b));
  }

};

std::shared_ptr<Blueprint> blueprint_from_string(const std::string &);

class Bunch {
private:
  i_t ore_=0;
  i_t clay_=0;
  i_t obsidian_=0;
  i_t geode_=0;
public:
  Bunch() = default;
  Bunch(i_t ore, i_t clay, i_t obsidian, i_t geode): ore_(ore), clay_(clay), obsidian_(obsidian), geode_(geode) {}
  void add(material robot, i_t amount=1) {
    switch(robot){
    case material::ore: ore_+=amount; break;
    case material::clay: clay_+=amount; break;
    case material::obsidian: obsidian_+=amount; break;
    case material::geode: geode_+=amount; break;
    default: {}
    }
  }
  void remove(material robot, i_t amount){
    switch(robot){
    case material::ore: ore_ -= amount; break;
    case material::clay: clay_ -= amount; break;
    case material::obsidian: obsidian_ -= amount; break;
    case material::geode: geode_ -= amount; break;
    default: {}
    }
  }
  [[nodiscard]] i_t get(material robot) const {
    switch(robot){
    case material::ore: return ore_;
    case material::clay: return clay_;
    case material::obsidian: return obsidian_;
    case material::geode: return geode_;
    default: return 0;
    }
  }
  void add(std::tuple<i_t, i_t, i_t, i_t> pile){
    ore_ += std::get<0>(pile);
    clay_ += std::get<1>(pile);
    obsidian_ += std::get<2>(pile);
    geode_ += std::get<3>(pile);
  }
  [[nodiscard]] std::tuple<i_t, i_t, i_t, i_t> get() const {
    return std::make_tuple(ore_, clay_, obsidian_, geode_);
  }

  friend std::ostream & operator<<(std::ostream & os, const Bunch & b){
    os << "[";
//    for (const auto mat: MATERIALS) os << " " << mat << ": " << b.get(mat);
    for (const auto mat: MATERIALS) os << std::setw(3) << b.get(mat);
    os << " ]";
    return os;
  }
};

class Miner {
public:
  using blueprint_t = std::shared_ptr<Blueprint>;
  using menagerie_t = Bunch;
  using stockpile_t = Bunch;
private:
  blueprint_t blueprint_;
  stockpile_t stockpile_={0,0,0,0};
  menagerie_t menagerie_={1,0,0,0};
public:
  explicit Miner(blueprint_t bp): blueprint_(std::move(bp)) {}
  Miner(blueprint_t b, stockpile_t s, menagerie_t m): blueprint_(std::move(b)), stockpile_(s), menagerie_(m) {}

  [[nodiscard]] std::vector<material> options(i_t duration, i_t time) const {
    std::vector<material> buildable;
    auto time_left = duration - time + 1;
    for (const auto & robot: MATERIALS){
      bool can_build{true}, should_build{true};
      for (const auto & ingredient: MATERIALS){
        if (stockpile_.get(ingredient) < blueprint_->cost(robot, ingredient)){
          can_build=false;
          break;
        }
      }
      if (robot != material::geode) {
        // do we have so much of a resource that building of that type would be a waste?
        auto max_cost = blueprint_->max(robot);
//        std::cout << "\nit costs no more than " << max_cost << " " << robot << " to build *a* robot and we have " << stockpile_.get(robot) << " stockpiled and " << menagerie_.get(robot) << " " << robot <<  " robots";
        if (stockpile_.get(robot) >= (max_cost * time_left)) {
//          std::cout << "\nThere's not enough time to need to build more " << robot << " robots because " << stockpile_.get(robot) << " >= " << max_cost * time_left;
          should_build = false;
        }
        // do we have so many of this robot already that we can't use extras?
        if (menagerie_.get(robot) >= max_cost) {
          should_build = false;
        }
      }
      if (can_build && should_build) buildable.push_back(robot);
    }
    // look for options which only make things worse: (let the blueprint handle this one)
//    if (buildable.size() > 1){
//      size_t j{0}, i{1};
//      while (i < buildable.size() && j < buildable.size()-1){
//        auto resources_required = blueprint_.compare(buildable[j], buildable[i]);
//        if (resources_required == std::weak_ordering::greater) {
//          buildable.erase(buildable.begin()+static_cast<long>(i));
//        }
//        if (resources_required == std::weak_ordering::less){
//          buildable.erase(buildable.begin()+static_cast<long>(j));
//        }
//        if (resources_required == std::weak_ordering::equivalent){
//          ++i;
//        }
//        if (i >= buildable.size() && ++j < buildable.size()-1){
//          i = j+1;
//        }
//      }
//    }


    // plus we can always build nothing to just gather resources
    buildable.push_back(material::none);
    return buildable;
  }

  void collect() {stockpile_.add(menagerie_.get());}
  [[nodiscard]] Miner build(material robot, i_t time_left) const {
    if (robot == material::none) return {blueprint_, stockpile_, menagerie_};
    // Construct a new stockpile and update its values to reflect the robot build cost
    auto new_stockpile = stockpile_;
    for (const auto & ingredient: MATERIALS) new_stockpile.remove(ingredient, blueprint_->cost(robot, ingredient));
    // Construct a new menagerie with the added robot
    auto new_menagerie = menagerie_;
    if (material::geode == robot){
      // instead of adding a geode robot to the state, just calculate how many geodes it *will* generate
      new_stockpile.add(robot, time_left > 0 ? time_left - 1 : 0);
    } else {
      new_menagerie.add(robot);
    }
    return {blueprint_, new_stockpile, new_menagerie};
  }

  [[nodiscard]] i_t score() const {return stockpile_.get(material::geode);}
  [[nodiscard]] stockpile_t stockpile() const {return stockpile_;}
  [[nodiscard]] menagerie_t menagerie() const {return menagerie_;}

//  [[nodiscard]] bool operator<(const Miner & o) const {return score() < o.score();}

  friend std::ostream & operator<<(std::ostream & os, const Miner & m) {
    os << "stock " << m.stockpile() << " robots " << m.menagerie();
    return os;
  }
  [[nodiscard]] std::weak_ordering operator<=>(const Miner & miner) const {
    const auto & m = miner.menagerie();
    for (const auto & mat: MATERIALS) // geode, obsidian, clay, ore
      if (auto ans = m.get(mat) <=> menagerie_.get(mat); ans != std::weak_ordering::equivalent)
        return ans;
    const auto & s = miner.stockpile();
    for (const auto & mat: MATERIALS) // geode, obsidian, clay, ore
      if (auto ans = s.get(mat) <=> stockpile_.get(mat); ans != std::weak_ordering::equivalent)
        return ans;
    return std::weak_ordering::equivalent;
  }
};

class Foreman {
public:
  using blueprint_t = Miner::blueprint_t;
private:
  Miner miner_;
	i_t geodes_=0;
public:
  explicit Foreman(blueprint_t bp): miner_(std::move(bp)) {}
	Foreman(blueprint_t bp, i_t time): miner_(std::move(bp)) {
    std::cout << "\n\n" << bp << "\n";
	  geodes_ = optimize(time);
    std::cout << "Best solution has " << geodes_ << " geodes after " << time << " minutes\n";
	}
	[[nodiscard]] i_t geodes() const {return geodes_;}

  [[nodiscard]] i_t optimize(i_t time) {
    auto all = strategize(time);
    return all.empty() ? 0 : std::max_element(all.begin(), all.end())->score();
  }
private:

  [[nodiscard]] std::vector<Miner> strategize(i_t duration){
    std::vector<Miner> states;
    states.push_back(miner_);
    i_t most_found_geodes{0};
    for (i_t time=0; time<duration; ++time){
      auto time_left = duration - time;
      std::vector<Miner> new_states;
      new_states.reserve(2 * states.size());
      i_t pruned{0};
      for (auto& state: states){
        // can this state *possibly* produce enough geodes?
        if (state.stockpile().get(material::geode) + time_left * (time_left - 1) / 2 < most_found_geodes){
          ++pruned;
          continue;
        }
        auto options = state.options(duration, time);
        state.collect();
        for (const auto & robot: options) new_states.push_back(state.build(robot, time_left));
      }
      // swap the new and old states
      std::swap(states, new_states);
      // look for duplicate states
      std::sort(states.begin(), states.end(), [](const auto & a, const auto & b){return a <=> b == std::weak_ordering::less;});
      auto found_states = states.size();
      states.erase(std::unique(states.begin(), states.end(), [](const auto & a, const auto & b){return a <=> b == std::weak_ordering::equivalent;}), states.end());
      most_found_geodes = std::max_element(states.begin(), states.end(), [](const auto & a, const auto &b){return a.stockpile().get(material::geode) < b.stockpile().get(material::geode);})->stockpile().get(material::geode);

      std::cout << "After" << std::setw(3) << time+1 << " of " << duration;
      std::cout << " : most found " << std::setw(10) << most_found_geodes;
      std::cout << " : pruned" << std::setw(10) << pruned << ": generated " << std::setw(10) << found_states;
      std::cout << " of which " << std::setw(10) << states.size() << " are unique\n";



//      for (const auto & state: states) std::cout << state << "\n";
    }
    std::cout << "Most found geodes ended as " << most_found_geodes << "\n";
    return states;
  }


};

} // aoc::robots
#endif
