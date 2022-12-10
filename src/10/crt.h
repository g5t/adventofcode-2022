#ifndef AOC_CRT_H
#define AOC_CRT_H

#include <numeric>

namespace aoc::crt {

enum class instruction {noop, addx};

class CPU {
private:
  int x_=1;
  unsigned cycles_=0;
  std::vector<int> signal_;

public:
  CPU() = default;

	auto x() const {return x_;}
  auto cycles() const {return cycles_;}
  std::vector<int> signal() const {return signal_;}
  int total() const {return std::accumulate(signal_.begin(), signal_.end(), 0);}

  [[nodiscard]] auto run(const std::vector<std::pair<instruction, int>>& inst) {
    for (const auto & p: inst){
      switch(p.first){
      case instruction::noop: tick(); break;
      case instruction::addx: tick(); tick(); x_ += p.second; break;
      }
    }
    return total();
  }

private:
  void tick() {
		before_tick();
    ++cycles_;
		after_tick();
  }
	virtual void before_tick() {}
	virtual void after_tick() {
    if ((cycles_ + 20) % 40 == 0) record();
	}
  void record() {
    signal_.push_back(static_cast<int>(cycles_) * x_);
  }
};

class CRT: public CPU {
	using row_t = std::array<char, 40>;
	using display_t = std::array<row_t, 6>;
private:
	display_t display_;

public:
	CRT() {
		for (auto & row: display_) for (auto & col: row) col = ' ';

	}

	const display_t & display() const {return display_;}

	friend std::ostream & operator<<(std::ostream& os, const CRT & crt){
		for (const auto & row: crt.display()){
			for (const auto & col: row) os << col;
			os << "\n";
		}
		return os;
	}

private:
	void before_tick() {
		// check if the sprite (x_) is within 1 of the pixel position
		auto row = (cycles() / 40) % 6;
		auto col = cycles() % 40;
		auto spr = (std::abs(static_cast<int>(col) - x()) < 2) ? '*' : ' ';
		display_[row][col] = spr;
	}
};

} // namespace aoc::crt
#endif
