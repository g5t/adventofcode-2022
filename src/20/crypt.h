#ifndef AOC_CRYPT_H
#define AOC_CRYPT_H

#include <vector>
#include <iostream>
#include <iomanip>

template<class T> void print_vector(const int w, const std::vector<T>& v){
  for (const auto & x: v) std::cout << std::setw(w) << x;
	std::cout << "\n";
}

namespace aoc::crypt {

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

template<class T>
class Item{
  using w_ptr_t = std::weak_ptr<Item>;
	using s_ptr_t = std::shared_ptr<Item>;
	using opt_t = std::optional<w_ptr_t>;
	using val_t = T;

	val_t value_;
	opt_t prev_ = std::nullopt;
	opt_t next_ = std::nullopt;

public:
  Item(val_t v): value_(v) {};
	
	bool has_prev() const {return prev_.has_value();}
	bool has_next() const {return next_.has_value();}
	s_ptr_t prev() const {return prev_.value().lock();}
	s_ptr_t next() const {return next_.value().lock();}
	void prev(s_ptr_t p) {prev_=p;}
	void next(s_ptr_t n) {next_=n;}

	val_t value() const {return value_;}

	friend std::ostream & operator<<(std::ostream & os, const Item<T>& x){
		os << x.value();
		return os;
	}
};

template<class T>
size_t mod(T i, T s) {
  if (i < 0) i -= (i/s) * s;
	if (i < 0) i += s;
	if (s <= i) i %= s;
	if (0 <= i && i < s) return static_cast<size_t>(i);
	throw std::runtime_error("Indexing problem");
}


template<class T>
class Mixer {
public:
  using item_t = std::shared_ptr<Item<T>>;
	using store_t = std::vector<item_t>;
private:
  store_t input_;
public:
  Mixer(const std::vector<T> & in, T key=1, T rounds=1){
	  auto N = in.size();
		input_.reserve(N);
		for (const auto & i: in) input_.push_back(std::make_shared<Item<T>>(i*key));
		auto sN = static_cast<int64_t>(N);
		for (int64_t i=0; i<sN; ++i){
		  input_[index(i)]->prev(input_[index(i-1)]);
			input_[index(i)]->next(input_[index(i+1)]);
		}
		for (T i=0; i<rounds; ++i) recipie();
	}

	const store_t & store() const {return input_;}
	size_t size() const {return input_.size();}
	size_t index(int64_t i) const {return mod(i, static_cast<int64_t>(size()));}
	size_t dist(int64_t i) const { return mod(i, static_cast<int64_t>(size())-1);}

	T coordinate_sum() const {
	  T s{0};
		// find the input index of value 0:
		auto zero_index = static_cast<size_t>(std::distance(input_.begin(), std::find_if(input_.begin(), input_.end(), [](const auto & x){return x->value()==0;})));
		// each coordinate is the 1,2,3, thousandth number *after the zero* in the reordered list:
		auto at = input_[zero_index];
		for (size_t c=0; c<3; ++c){
		  for (size_t i=0; i<(1000 % size()); ++i) at = at->next();
			s += at->value();
		}
		return s;
	}


	friend std::ostream & operator<<(std::ostream & os, const Mixer<T> m){
		const auto & first = m.store()[0];
		os << first->value();
		auto next = first->next();
		while (first != next){
			os << ", " << next->value();
			next = next->next();
		}
		return os;
	}
private:
	void recipie() {
//    std::cout << "Initial arrangement:\n" << *this << "\n";
		for (const auto & node: input_) move(node);
	}

	void move(const item_t & start) {
	  auto how_far = [&](auto x){
		  return static_cast<size_t>(std::abs(x)) % (size() - 1);
		};
    
		auto num = how_far(start->value());
		if (num == 0) return;

    auto before = start;
		auto after = start;

		if (start->value()>0){
		  for (size_t i=0; i<num; ++i) before = before->next();
			after = before->next();
		} else {
		  for (size_t i=0; i<num; ++i) after = after->prev();
			before = after->prev();
		}

		// take out of current position:
		auto old_prev = start->prev();
		auto old_next = start->next();
		old_prev->next(old_next);
		old_next->prev(old_prev);

		// insert in the new position
		before->next(start);
		start->prev(before);
		start->next(after);
		after->prev(start);

//		std::cout << start->value() << " moves between " << before->value() << " and " << after->value() << ":\n" << *this << "\n";
	}


};

} // namespace aoc::crypt
#endif
