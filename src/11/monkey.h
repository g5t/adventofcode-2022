#ifndef AOC_MONKEY_H
#define AOC_MONKEY_H

#include <set>
#include <numeric> 

namespace aoc::monkey {

//enum class op {noop, multiply, divide, add, subtract, square};
enum class op {multiply, add, square};

template<class T>
T set_lcm(const std::set<T>& vals) {
	std::set<T> divisors;
	for (T val: vals) for (T i=2; i<val+1; ++i) if (val%i==0) divisors.insert(i);
	std::set<T> primes;
	for (T d: divisors){
		bool is_prime{true};
		for (T i=2; i<d/2; ++i) if (d%i==0) is_prime=false;
		if (is_prime) primes.insert(d);
	}

	return std::reduce(primes.begin(), primes.end(), T(1), [](T a, T b){return a*b;});
}

template<class val_t>
class Monkey {
public:
  using ind_t = unsigned;
	using throw_t = std::pair<ind_t, val_t>;
	using worry_t = std::vector<val_t>;
	using op_t = std::pair<op, val_t>;
	using branch_t = std::pair<ind_t, ind_t>;

  Monkey(worry_t w, op_t o, val_t div, branch_t br):
    worry_(std::move(w)), op_(o), div_(div), branch_(br)
  {
  }

	worry_t items() const {return worry_;}

  void receive(val_t item) {
    worry_.push_back(item);
  }

  std::vector<throw_t> inspect(val_t relief, val_t modulus) {
    std::vector<throw_t> thrown;
    for (auto nw: worry_){
			++inspections_;
      switch(op_.first){
			case op::multiply: nw *= op_.second; break;
			case op::add: nw += op_.second; break;
			case op::square: nw *= nw; break;
      }
			// apply the relief:
			if (relief > 0) nw /= relief;
			if (modulus > 1) nw %= modulus;
      // decide what to do:
			auto target = (nw % div_ == 0) ? branch_.first : branch_.second;
			thrown.emplace_back(target, nw);
    }
    // all items have been inspected and thrown; so empty our storage
    worry_.clear();
    // and tell the Troop what we've done
    return thrown;
  }

  val_t inspections() const {return inspections_;}

	op_t op() const {return op_;}
	val_t divisor() const {return div_;}

	friend std::ostream & operator<<(std::ostream & os, const Monkey<val_t>& m){
	  auto o = m.op();
		switch(o.first){
		case op::multiply: os << "*" << o.second; break;
		case op::add: os << "+" << o.second; break;
		case op::square: os << "^2"; break;
		}
		os << " %" << m.divisor() << "\n";
		return os;
	}

private:
  worry_t worry_;
  op_t op_;
  val_t div_=1;
  branch_t branch_={0,0};
  val_t inspections_=0;
};

template<class val_t>
class Troop {
public:
	Troop(std::vector<Monkey<val_t>>&& tr, val_t r=3, val_t m=1)
	: troop_(std::move(tr)), red_(r) {
		if (0==m) {
			std::set<val_t> divs;
			for (const auto & monkey: troop_) divs.insert(monkey.divisor());
			m = set_lcm(divs);
		}
		mod_=m;
	}

  void run_round(){
		//unsigned index{0};
    for (auto & monkey: troop_){
      auto items = monkey.inspect(red_, mod_);
      for (const auto & item: items){
        troop_[item.first].receive(item.second);
      }
    }
  }

	std::vector<val_t> inspections() const {
    std::vector<val_t> ins;
    std::transform(troop_.begin(), troop_.end(), std::back_inserter(ins), [](auto & m){return m.inspections();});
		return ins;
	}

  val_t business() const {
		auto ins = inspections();
    std::partial_sort(ins.begin(), ins.begin()+2, ins.end(), [](auto a, auto b){return a>b;});
    return ins[0] * ins[1];
  }

	std::vector<Monkey<val_t>> monkeys() const {return troop_;}

	friend std::ostream & operator<<(std::ostream & os, const Troop<val_t> & t){
		for (const auto & val: t.inspections()) os << " " << val;
		os << "\n";
    return os;
	}

private:
  std::vector<Monkey<val_t>> troop_;
	val_t mod_=1;
	val_t red_=3;
};

} // namespace aoc::monkey
#endif
