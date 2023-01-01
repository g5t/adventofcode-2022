#ifndef AOC_ROLL_H
#define AOC_ROLL_H
namespace aoc{

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

} // namespace aoc
#endif
