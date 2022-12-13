#ifndef AOC_TOPO_H
#define AOC_TOPO_H

#include <optional>
#include <algorithm>
#include <vector>

namespace aoc::topo {

template<class T>
T manhattan_distance(std::pair<T, T> a, std::pair<T, T> b){
	T dist{0};
	dist += (a.first > b.first) ? (a.first - b.first) : (b.first - a.first);
	dist += (a.second > b.second) ? (a.second - b.second) : (b.second - b.first);
	return dist;
}

class Tile{
	int altitude_;
	std::optional<int> cost_=std::nullopt;

public:
	Tile(int a): altitude_(a) {}
	Tile(char a){
		if ('S' == a){
			cost_ = -1;
			a = 'a';
		}
		if ('E' == a){
			cost_ = 0;
			a = 'z';
		}
		altitude_ = a - 'a';
	}

	bool known() const { return cost_.has_value() && cost_.value() >= 0;}
	bool is_target() const {return cost_.has_value() && cost_.value() < 0;}

	void unset_cost() {cost_=std::nullopt;}
	void set_cost(int cost) {cost_=cost;}
	int get_cost() const {return cost_.value();}

	int altitude() const {return altitude_;}

	int operator-(const Tile & other) const {
		return altitude_ - other.altitude();
	}

	bool operator==(const Tile & other) const {
		return altitude_ == other.altitude();
	}

	friend std::ostream & operator<<(std::ostream & os, const Tile & tile){
		os << static_cast<char>(tile.altitude() + 'a');
		return os;
	}
};

class PathField{
public:
	using ind_t = unsigned;
  using dir_t = int;
	using tile_t = std::optional<dir_t>;
	using row_t = std::vector<tile_t>;
	using map_t = std::vector<row_t>;
	using coord_t = std::pair<ind_t, ind_t>;

private:
	map_t map_;

public:
template<class T>
	PathField(T rows, T cols) {
		map_.resize(rows);
		for (auto & r: map_){
		 r.resize(cols);
		 for (auto & c: r) c = std::nullopt;
		}
	}

	void set_towards(coord_t a, coord_t b){
		if (a.first == b.first && a.second < b.second){ /*>*/ map_[a.first][a.second] = 2; return;}
		if (a.first == b.first && a.second > b.second){ /*<*/ map_[a.first][a.second] = 6; return;}
		if (a.first > b.first && a.second == b.second){ /*^*/ map_[a.first][a.second] = 0; return;}
		if (a.first < b.first && a.second == b.second){ /*v*/ map_[a.first][a.second] = 4; return;}
		// 
		std::string msg = "No direction set for ";
		msg += "(" + std::to_string(a.first) + "," + std::to_string(a.second) + ")";
		msg += " --> ";
		msg += "(" + std::to_string(b.first) + "," + std::to_string(b.second) + ")";
		throw std::runtime_error(msg);
	}
	
	const map_t & map_ref() const {return map_;}

	friend std::ostream & operator<<(std::ostream & os, const PathField & pf){
		for (auto & r: pf.map_ref()){
			for (auto & c: r) {
				if (c.has_value()) switch (c.value()){
				case 0: os << '^'; break;
				case 1: os << '/'; break;
				case 2: os << '>'; break;
				case 3: os << '\\'; break;
				case 4: os << 'v'; break;
				case 5: os << '%'; break;
				case 6: os << '<'; break;
				case 7: os << '&'; break;
				default: os << '?';
				} else {
				os << '.';
				}
			}
			os << "\n";
	  }
	  return os;
	}

	std::optional<coord_t> next(coord_t at) const {
		auto v = map_[at.first][at.second];
		if (!v.has_value()) return std::nullopt;
		
		auto vv = v.value();
		// add/subtract (1,0)/(0,1) for the neighbors
		if ((vv < 2) || (vv > 6)) at.first -= 1;
		if ((vv > 2) && (vv < 6)) at.first += 1;
		if ((vv > 0) && (vv < 4)) at.second += 1;
		if ((vv > 4) && (vv < 8)) at.second -= 1;

		return {at};
	}

	std::vector<coord_t> follow(coord_t from, coord_t to) const {
		std::vector<coord_t> path{from};

		// allow for missing path information in case *there is no solution*
		if (!map_[from.first][from.second].has_value()) return path;
		
		while (path.back().first != to.first || path.back().second != to.second){
			auto v = next(path.back());
			if (!v.has_value()) throw std::runtime_error("No path to follow!");
			path.push_back(v.value());
		}
		return path;
	}

	bool connects(coord_t from, coord_t to) const {
		while (from.first != to.first || from.second != to.second){
			auto v = next(from);
			if (!v.has_value()) return false;
			from = v.value();
		}
		return true;

	}

};

class Map{
public:
	using tile_t = Tile;
	using row_t = std::vector<tile_t>;
	using map_t = std::vector<row_t>;
	using ind_t = unsigned;
	using coord_t = std::pair<ind_t, ind_t>;
	using path_t = PathField;
	
private:
  map_t map_;
	path_t path_;
	coord_t start_;
	coord_t end_;

public:
  Map(const std::vector<std::string> & m): path_(m.size(), m[0].size()){
		for (const auto & r: m){
			row_t row;
			for (const auto & c: r) row.emplace_back(c);
			map_.push_back(row);
		}
		auto n_row = map_.size();
		auto n_col = map_[0].size();
		for (const auto & r: map_) if (r.size() != n_col) throw std::runtime_error("Map must be rectangular");
		// to start only one tile is known (the start)
		// and one tile is the target (the end)
		for (ind_t r=0; r<n_row; ++r) for (ind_t c=0; c<n_col; ++c) {
			if (map_[r][c].known()) end_={r,c};
			if (map_[r][c].is_target()) start_={r,c};
	  }
	}

	coord_t start() const {return start_;}
	coord_t end() const {return end_;}
	void set_start(coord_t x) {
		(*this)[start_].unset_cost();
		(*this)[x].set_cost(0);
		start_ = x;
	}

	const map_t & map_ref() const {return map_;}
	const path_t & path_ref() const {return path_;}

	friend std::ostream & operator<<(std::ostream & os, const Map & m){
		for (const auto & r: m.map_ref()){
			for (const auto & t: r) os << t;
			os << "\n";
		}
		return os;
	}

	std::vector<coord_t> neighbors(coord_t x) const {
		std::vector<coord_t> n;
		if (x.first > 0) n.emplace_back(x.first-1, x.second);
		if (x.first < map_.size() - 1) n.emplace_back(x.first+1, x.second);
		if (x.second > 0) n.emplace_back(x.first, x.second-1);
		if (x.second < map_[0].size() - 1) n.emplace_back(x.first, x.second+1);
		return n;
	}

	const tile_t & operator[](const coord_t & coord) const {
		return map_[coord.first][coord.second];
	}
	tile_t & operator[](const coord_t & coord) { return map_[coord.first][coord.second]; }

	bool reachable(const coord_t & from, const coord_t & to) const {
		auto diff = (*this)[to] - (*this)[from];
		return diff <= 1;
	}

	std::vector<coord_t> path() const {
		auto crumbs = path_.follow(start_, end_);
		return crumbs;
	}

	std::vector<coord_t> blaze(bool stop_at_zero = false) {
		// Djikstra's algorithm from end_
	  std::vector<coord_t> border{end_};
		while (!border.empty()) {
			// pick the lowest-cost border tile
			std::partial_sort(border.begin(), border.begin()+1, border.end(), 
				[&](const auto & a, const auto & b){return (*this)[a].get_cost() < (*this)[b].get_cost();});
			auto cc = border.front();
			// remove the cheapest cost from the border
			border.erase(border.begin());

			// Special shortcut for part-two:
			if (stop_at_zero && (*this)[cc].altitude() == 0) continue;

			// find its neighbors
			auto next = neighbors(cc);

			// filter those for which can be reached (in reverse)
			next.erase(std::remove_if(next.begin(), next.end(), [&](const auto & nb){return !reachable(nb, cc);}), next.end());

			// and which have not been visited already
			next.erase(std::remove_if(next.begin(), next.end(), [&](const auto & nb){return (*this)[nb].known();}), next.end());

			for (auto & n: next){
			  // calculate their cost (this tile's cost +1)
			  auto cost = (*this)[cc].get_cost() + 1;
				// if the cost is less than their current value:
				auto & nt = (*this)[n];
				if (!nt.known() || nt.get_cost() > cost){
			    // set their cost
					nt.set_cost(cost);
					// and keep the pointer back to this tile
					path_.set_towards(n, cc);
					// and add the neighbor to border if it isn't already present
					if (std::find(border.begin(), border.end(), n) == border.end()){
					  border.push_back(n);
					}
				}
			}
		}
		return path();
	}

};

class Hikes{
public:
	using map_t = Map;
	using coord_t = map_t::coord_t;
	using hike_t = std::vector<coord_t>;
	using hikes_t = std::vector<hike_t>;

private:
  map_t map_;
	hike_t starts_;
	hikes_t hikes_;

public:
  Hikes(const std::vector<std::string> & m): map_(m) {
		starts_.push_back(map_.start());
		auto start = starts_.front();
		auto base = map_.map_ref();

		for (size_t i=0; i<base.size(); ++i){
			for (size_t j=0; j<base[0].size(); ++j){
				if (!(start.first == i && start.second == j) && map_[start] == map_[std::make_pair(i,j)]){
					starts_.emplace_back(i,j);
				}
			}
		}
		//std::cout << *this;
	}

	const map_t & map_ref() const {return map_;}
	const hike_t & starts_ref() const {return starts_;}
	const hikes_t & hikes_ref() const {return hikes_;}

	friend std::ostream & operator<<(std::ostream & os, const Hikes & h){
		os << h.map_ref();
		for (auto & s: h.starts_ref()) os << "(" << s.first << "," << s.second << ") ";
		os << "\n";
		return os;
	}

	hike_t optimal() {
		// fill in the map's PathField
		map_.blaze(/*stop_at_zero=*/true);
		auto end = map_.end();

		auto pr = map_.path_ref();

		//std::cout << pr;

		// Remove possible starting points which do not connect to the end
		starts_.erase(std::remove_if(starts_.begin(), starts_.end(), [&](auto & x){return !pr.connects(x, end);}), starts_.end());

		// Find the remaining paths
		hikes_.reserve(starts_.size());
		std::transform(starts_.begin(), starts_.end(), std::back_inserter(hikes_), [&](auto & x){return pr.follow(x, end);});

		// And pull-out the shortest one
		std::partial_sort(hikes_.begin(), hikes_.begin()+1, hikes_.end(), [](auto & a, auto &b){return a.size() < b.size();});
		return hikes_.front();
	}
			

};

} // namespace aoc::topo
#endif
