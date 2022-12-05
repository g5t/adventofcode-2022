#ifndef AOC_STACKS_H
#define AOC_STACKS_H

#include <stack>
#include <regex>
#include <iostream>

#include "read.h"

namespace aoc::stacks {

template <class T>
class Stacks {
public:
//  using stack_t = std::stack<T>;
	using stack_t = std::vector<T>;
private:
	std::vector<stack_t> stacks_;

public:
  Stacks(std::vector<stack_t> && s): stacks_(std::move(s)) {};

	Stacks(const Stacks<T>& s){
		stacks_.reserve(s.size());
		//for (const auto & t : s.stacks()) stacks_.emplace_back(t);
		for (const auto & t: s.stacks()){
			stack_t tt = t;
		  stacks_.push_back(tt);
		}
	}

	bool move(size_t num, size_t from, size_t to) {
		if (from >= stacks_.size()) throw std::runtime_error("Invalid from index");
		if (to >= stacks_.size()) throw std::runtime_error("Invalid to index");
		if (num > size(from)) throw std::runtime_error("Moving too many from stack");

		size_t start = size(from) - num;
		for (size_t i=start; i < size(from); ++i) stacks_[to].emplace_back(stacks_[from][i]);
		stacks_[from].resize(start);
		
		return true;
	}

	bool move(size_t from, size_t to) {
		if (from >= stacks_.size()) throw std::runtime_error("Invalid from index");
		if (to >= stacks_.size()) throw std::runtime_error("Invalid to index");
		if (stacks_[from].empty()) return false;

		// .push() would add a const ref, which then gets invalidated with pop?
		stacks_[to].emplace_back(stacks_[from].back());
		stacks_[from].pop_back();

		return true;
	}

	const std::vector<stack_t>& stacks() const {return stacks_;} 

	const T& top(size_t index) { return stacks_[index].back(); }
	void pop(size_t index) { return stacks_[index].pop_back(); }

	size_t size() const {return stacks_.size();}
	size_t size(size_t index) {return stacks_[index].size();}

	std::vector<T> on_top() const {
		std::vector<T> tops;
		for (const auto & s: stacks_) tops.push_back(s.back());
		return tops;
	}

	size_t max_height() const {
		size_t h{0};
		for (const auto & s: stacks_) if (s.size() > h) h = s.size();
		return h;
	}

	friend std::ostream & operator<<(std::ostream & os, const Stacks<T>& s) {
		Stacks<T> stack(s);
		for (auto row = stack.max_height(); row-- > 0; ){
			 for (size_t i=0; i < stack.size(); ++i){
				if (stack.size(i) > row){
				  os << "[" << stack.top(i) << "]";
				  stack.pop(i);
				} else {
				  os << "   ";
				}
				os << " ";
			 }
			 os << "\n";
		}
		for (size_t i=0; i < stack.size(); ++i) os << " " << i << "  ";
		os << "\n";
		return os;
}


};





/* Read a fixed-width stack map like
 *
 * [A]     [B]
 * [C] [D] [E]
 *  1   2   3
 * */
template <class T, class F>
Stacks<T> read(const std::vector<std::string>& map, F fn){
	const std::string & last = map.back();

	std::vector<std::string> columns = aoc::line_to_columns(last);

	std::vector<size_t> positions;
	for (const auto & c: columns){
		auto pos = last.find(c);
		if (pos == std::string::npos) {
			std::string msg = "This should be impossible, but ";
			msg += c;
			msg += " is not in\n";
			msg += last;
			throw std::runtime_error(msg);
		}
		positions.push_back(pos);
	}

	size_t ncols = columns.size();
	std::vector<typename Stacks<T>::stack_t> stacks(ncols);

  for (auto ptr = map.rbegin()+1; ptr != map.rend(); ++ptr){
		for (size_t i=0; i<ncols; ++i){
			auto line = *ptr;
			auto val = line[positions[i]];
			if (val != ' ') {
				T res = fn(val);
				stacks[i].push_back(res);
			}
		}
	}

	return {std::move(stacks)};
}

} // end namespace

//template<class T>
//friend std::ostream & operator<<(std::ostream & os, aoc::stacks::Stacks<T> & s) {
//		aoc::stacks::Stacks<T> stack(s);
//		for (auto row = stack.max_height(); row-- > 0; ){
//			 for (size_t i=0; i < stack.size(); ++i){
//				if (stack.size(i) > row){
//				  os << "[" << stack.top(i) << "]";
//				  stack.pop(i);
//				} else {
//				  os << "   ";
//				}
//				os << " ";
//			 }
//			 os << "\n";
//		}
//		return os;
//}

#endif
