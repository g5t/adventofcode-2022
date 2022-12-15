#ifndef AOC_SENSOR_H
#define AOC_SENSOR_H
#include <map>
#include <set>
#include <vector>
#include <algorithm>
#include <string>
#include <iostream>

#include "libaoc.h"

namespace aoc::sensor{

// since std::abs is not defined for unsigned types
template<class T>
static T local_abs(T a, T b){
  return (a > b) ? a - b : b - a;
}

class Sensor {
public:
  using ind_t = long long;
  using coord_t = std::pair<ind_t, ind_t>;
    using range_t = aoc::ranges::Range<ind_t>;

private:
  coord_t sensor_;
  coord_t beacon_;
  ind_t dist_=0;

public:
  Sensor(coord_t s, coord_t b): sensor_(s), beacon_(b) {
    dist_ = local_abs(sensor_.first, beacon_.first) + local_abs(sensor_.second, beacon_.second);
  }
  
  coord_t sensor() const {return sensor_;}
  coord_t beacon() const {return beacon_;}

  // Maybe this should be something clever with ranges ...
  std::pair<bool, range_t> excluded_with_y(ind_t y) const {
    // find all x which produce points (x,y) with Manhattan distance <= dist_
    ind_t rem = dist_ - local_abs(sensor_.second, y);
    return {rem > 0, range_t(sensor_.first - rem, sensor_.first + rem)};
  }

  friend std::ostream & operator<<(std::ostream & os, const Sensor & s){
	  os << "S@(" << s.sensor().first << "," << s.sensor().second << "):";
		os << "B@(" << s.beacon().first << "," << s.beacon().second << ")";
    return os;
	}

};

class Network {
  using net_t = std::vector<Sensor>;
  net_t net_;
public:
  Network() = default;
  Network(net_t && n): net_(std::move(n)) {}

	const net_t & network() const {return net_;}

	aoc::ranges::RangeSet<Sensor::ind_t> in_range_with_y(Sensor::ind_t y) const {
    aoc::ranges::RangeSet<Sensor::ind_t> inrange(net_.size());
    for (const auto & sensor: net_){
      const auto & res = sensor.excluded_with_y(y);
      if (res.first) {
        inrange.add(res.second);
      }
    }
		return inrange;
	}

  Sensor::ind_t count_excluded_with_y(Sensor::ind_t y) const {
    aoc::ranges::RangeSet<Sensor::ind_t> inrange(net_.size());
    std::set<Sensor::ind_t> beacon;
    //beacon.reserve(net_.size());
    for (const auto & sensor: net_){
      const auto & res = sensor.excluded_with_y(y);
      if (res.first) {
        inrange.add(res.second);
      }
      // collect the beacons *on* the line of interest
      const auto & bec = sensor.beacon();
      if (bec.second == y) beacon.insert(bec.first);
    }

		for (auto b: beacon) std::cout << b << " ";
		auto beacons = std::transform_reduce(beacon.begin(), beacon.end(), 
		                                     0u, [](size_t a, size_t b){return a+b;}, 
																				 [&](Sensor::ind_t bec){return inrange.contains(bec);});

    auto count = inrange.size() - beacons;
    return static_cast<Sensor::ind_t>(count);
  }

	Sensor::ind_t find_distress_beacon(Sensor::ind_t bound) const {
	  // naive way:
		auto xrange = Sensor::range_t(0, bound);
		for (Sensor::ind_t y=0; y<bound; ++y){
			auto inr = in_range_with_y(y);
			auto rem = xrange - inr;
			if (rem.size()){
				auto x = rem.set()[0].first();
				return 4'000'000 * x + y;
			}
		}
		return -1;
	}

	friend std::ostream & operator<<(std::ostream & os, const Network & n){
	  for (const auto & s: n.network()) os << s << "\n";
		return os;
	}
};

Sensor from_string(const std::string & line);

} // aoc::sensor
#endif
