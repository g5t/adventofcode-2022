#ifndef AOC_MONKEY_MATH_H
#define AOC_MONKEY_MATH_H
#include <vector>
#include <map>
#include <memory>
#include <iostream>
#include <variant>
#include <sstream>
#include <optional>
#include <algorithm>

namespace aoc::monkey {
enum class operation {plus, minus, times, divide, equals};

template<class T> T operate(T a, operation op, T b){
 switch(op){
 case operation::plus: return a+b;
 case operation::minus: return a-b;
 case operation::times: return a*b;
 case operation::divide: return a/b;
 case operation::equals: return a==b ? T(1) : T(0);
 }
}
} // namespace aoc::monkey
std::ostream & operator<<(std::ostream &, aoc::monkey::operation);
std::stringstream & operator<<(std::stringstream &, aoc::monkey::operation);

namespace aoc::monkey {
template <class T>
class Monkey {
public:
  using val_t = std::optional<T>;
  using s_ptr_t = std::shared_ptr<Monkey<T>>;
  using w_ptr_t = std::weak_ptr<Monkey<T>>;
  using pack_t = std::tuple<w_ptr_t, operation, w_ptr_t>;
  using data_t = std::variant<val_t, pack_t>;
private:
  data_t data_=std::nullopt;
public:
  Monkey() = default;
  //Monkey(T v): data_(v) {};
  //Monkey(T a, operation op, T b) data_(operate(a,op,b)) {};
  //Monkey(s_ptr_t a, operation op, s_ptr_t b): data_(std::make_tuple<pack_t>(a, op, b)) {}

  void set(T v) {data_=v;}
  void set(s_ptr_t a, operation op, s_ptr_t b) {
    w_ptr_t a_ptr{a}, b_ptr{b};
    data_=std::make_tuple(a_ptr, op, b_ptr);
  }

  bool is_value() const {return std::holds_alternative<val_t>(data_) && std::get<val_t>(data_).has_value();}
  T value() const {return std::get<val_t>(data_).value();}

  bool is_variable() const {return std::holds_alternative<val_t>(data_) && !std::get<val_t>(data_).has_value();}
  bool is_operation() const {return std::holds_alternative<pack_t>(data_);}

  pack_t operation() const {return std::get<pack_t>(data_);}

  bool execute() {
    if (is_operation()){
      auto pack = std::get<pack_t>(data_);
      auto mona = std::get<0>(pack).lock();
      auto monb = std::get<2>(pack).lock();
      if (mona->is_value() && monb->is_value())
        data_=operate(mona->value(), std::get<1>(pack), monb->value());
    }
    return is_value();
  }

  T solve(T value) {
    if (is_value()) throw std::runtime_error("Can not solve [value] == [value]!");
    if (is_variable()){
      data_ = value;
      return value;
    }
    // must be an operation
    auto [w_a, op, w_b] = operation();
    auto a = w_a.lock();
    auto b = w_b.lock();
    if (a->is_operation() && b->is_operation()) throw std::runtime_error("too complicated for me ...");
    if (a->is_value() && b->is_value()) throw std::runtime_error("Both left and right should not be values at this point");

    if (b->is_operation()) {
      if (a->is_value()){
        /* v = a + f(left, right) -> f() = v - a
         * v = a - f(left, right) -> v + f() = a -> f() = a - v
         * v = a * f(left, right) -> f() = v / a
         * v = a / f(left, right) -> v * f() = a -> f() = a / v
         * */
        auto av = a->value();
        switch(op){
        case operation::plus: return b->solve(value - av);
        case operation::minus: return b->solve(av - value);
        case operation::times: return b->solve(value / av);
        case operation::divide: return b->solve(av / value);
        default: throw std::runtime_error("Intentionally unhandled impossible case");
        }
      } else {
        /* This case should be impossible since we only ever have one unknown
         * and and operation f(left, right) would have been resolved to a value
         * already. So no worries!
         * */
        throw std::runtime_error("This case should have been resolved already");
      }
    }
    if (b->is_value()){
      // value = (a->left() {+-/*} a->right() {+-/*} b->value();
      //   * OR *
      // value = a->variable() {+-/*} b->value()
      auto bv = b->value();
      switch(op){
      case operation::plus: return a->solve(value - bv);
      case operation::minus: return a->solve(value + bv);
      case operation::times: return a->solve(value / bv);
      case operation::divide: return a->solve(value * bv);
      default: throw std::runtime_error("Intentionally unhandled impossible case");
      }
    }
    throw std::runtime_error("expected all cases to be handled by now");
  }
};


class Troop {
public:
  using val_t = int64_t;
  using monkey_t = Monkey<val_t>;
  using ptr_t = monkey_t::s_ptr_t;
  using troop_t = std::map<std::string, ptr_t>;
private:
  troop_t troop_;
public:
  Troop(const std::vector<std::string> & lines, int part=1) {
    for (const auto & line: lines){
      auto name = line.substr(0,4);
      troop_.insert({name, std::make_shared<monkey_t>()});
    }
    // 
    for (const auto & line: lines){
      auto name = line.substr(0,4);
      bool is_root = (1 == part) ? false : name == "root";
      bool is_humn = (1 == part) ? false : name == "humn";

      auto m_ptr = troop_.at(name);
      auto rest = line.substr(6);
      if (auto pos = rest.find_first_of("+-*/"); pos != std::string::npos){
        auto a = rest.substr(0,4);
        auto b = rest.substr(7,4);
        operation op = operation::plus;
        if (is_root) {
          op = operation::equals;
        } else {
          switch(rest[5]){
          case '+': op = operation::plus; break;
          case '-': op = operation::minus; break;
          case '*': op = operation::times; break;
          case '/': op = operation::divide; break;
          }
        }
        m_ptr->set(troop_.at(a), op, troop_.at(b));
      } else {
        // this is a value, convert it to an integer
        if (!is_humn)  m_ptr->set(std::stoll(rest));
      }
    }
  }

  std::string to_string() const {
    std::stringstream ss;
    for (const auto & [name, ptr]: troop_){
      ss << name << ": ";
      if (ptr->is_variable()) ss << "{unknown}";
      if (ptr->is_value()) ss << ptr->value();
      if (ptr->is_operation()){
        const auto & [wa, op, wb] = ptr->operation();
        auto a = wa.lock();
        auto b = wb.lock();
        for (const auto & [n, p]: troop_) if (a == p) ss << n;
        ss << " " << op << " ";
        for (const auto & [n, p]: troop_) if (b == p) ss << n;
      }
      ss << "\n";
    }
    return ss.str();
  }

  std::string monkey_string(ptr_t p) const {
    std::stringstream ss;
    if (p->is_variable()) {
      auto ptr = std::find_if(troop_.begin(), troop_.end(), [&](const auto & v){return v.second == p;});
      auto name = (ptr != troop_.begin() ? ptr->first : "UNKNOWN MONKEY");
      ss << name;
    }
    if (p->is_value()) ss << p->value();
    if (p->is_operation()) {
      const auto & [wa, op, wb] = p->operation();
      ss << "(" << monkey_string(wa.lock());
      ss << " " << op << " ";
      ss << monkey_string(wb.lock()) << ")";
    }
    return ss.str();
  }
  

  std::string equation_string() const {
    return monkey_string(troop_.at("root"));
  }

  friend std::ostream & operator<<(std::ostream & os, const Troop & tr){
    os << tr.to_string();
    return os;
  }

  void execute() {
    std::vector<ptr_t> stack;
    stack.reserve(troop_.size());
    for (auto & [name, ptr]: troop_) if (!ptr->is_value()) stack.push_back(ptr);
    while (!stack.empty()){
      for (auto & ptr: stack) ptr->execute();
      stack.erase(std::remove_if(stack.begin(), stack.end(), [](const auto & ptr){return ptr->is_value();}), stack.end());
    }
  }

  void simplify() {
    std::vector<ptr_t> stack;
    stack.reserve(troop_.size());
    for (auto & [name, ptr]: troop_) if (!ptr->is_value()) stack.push_back(ptr);
    auto last_size = stack.size() + 1;
    while (stack.size() != last_size){
      last_size = stack.size();
      for (auto & ptr: stack) ptr->execute();
      stack.erase(std::remove_if(stack.begin(), stack.end(), [](const auto & ptr){return ptr->is_value();}), stack.end());
    }
  }

  val_t solve(){
    simplify();
    // pull out f(a, b) == value OR value == f(a, b)
    auto [lhs, op, rhs] = at("root")->operation();
    // ensure we have things the right-way-round
    if (rhs.lock()->is_value()) std::swap(lhs, rhs);
    // and traverse f(a, b) until we find the humn variable, returning its value
    return rhs.lock()->solve(lhs.lock()->value());
  }

  ptr_t at(const std::string & name) const {return troop_.at(name);}
};


} // namespace aoc::monkey
#endif
