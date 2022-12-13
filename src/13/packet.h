#ifndef AOC_PACKET_H
#define AOC_PACKET_H
#include <optional>
#include <variant>
#include <vector>
#include <memory>
#include <sstream>
#include <algorithm>

namespace aoc::packet{

class Packet: public std::variant<std::vector<Packet>, int> {
  // Use the variant constructor
  using std::variant<std::vector<Packet>, int>::variant;

  // list extractor
  std::vector<Packet>& list() {return std::get<0>(*this);}
  [[nodiscard]] const std::vector<Packet>& list() const {return std::get<0>(*this);}
  // value extractor
  int& value() {return std::get<1>(*this);}
  [[nodiscard]] const int & value() const {return std::get<1>(*this);}
  [[nodiscard]] bool is_value() const {return std::holds_alternative<int>(*this);}
  [[nodiscard]] std::vector<Packet> value_list() const {return {value()};}

  friend std::ostream & operator<<(std::ostream & os, const Packet & op){
    if (op.is_value()){
      os << op.value();
    } else {
      auto & list = op.list();
      os << "[";
      std::stringstream ss;
      for (auto & x: list) ss << x << ",";
      os << ss.str().substr(0, ss.str().size()-1) << "]";
    }
    return os;
  }
public:
  [[nodiscard]] std::weak_ordering operator<=>(const Packet & op) const {
    if (is_value() && op.is_value()) return value() <=> op.value();
    // All other cases require two lists:
    auto left = is_value() ? value_list() : list();
    auto right = op.is_value() ? op.value_list() : op.list();
    size_t i{0};
    for (; i<left.size() && i<right.size(); ++i){
      if (auto res = left[i] <=> right[i]; res != std::weak_ordering::equivalent)
        return res;
    }
    return left.size() <=> right.size();
  }
};

Packet from_string(const std::string & line);

class TwoPacket {
  Packet left_;
  Packet right_;

public:
  explicit TwoPacket(const std::pair<std::string,std::string> & p):
    left_(from_string(p.first)), right_(from_string(p.second)) {}

  [[nodiscard]] bool ok() const {
    return (left_ <=> right_) == std::weak_ordering::less;
  }
  [[nodiscard]] const Packet & left() const {return left_;}
  [[nodiscard]] const Packet & right() const {return right_;}

  friend std::ostream & operator<<(std::ostream & os, const TwoPacket & pack){
    os << pack.left() << "\n" << pack.right() << "\n";
    return os;
  }

};

} // namespace aoc::packet

#endif
