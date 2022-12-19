#include <regex>
#include <iostream>
#include <libaoc.h>
#include "steam.h"

using namespace aoc::steam;

std::tuple<Cave, std::string, std::string> cave_from_string(const std::string & line){
  std::regex cave_regex("([A-Z][A-Z])[a-z ]*=([0-9]+);[a-z ]*([A-Z, ]*)");
  auto cave_begin = std::sregex_iterator(line.begin(), line.end(), cave_regex);
  auto cave_end = std::sregex_iterator();
  auto count_info = std::distance(cave_begin, cave_end);
  
  if (count_info != 1){
    std::string msg = "Regex parsing failed for '";
    msg += line;
    msg += "'";
    throw std::runtime_error(msg);
  }

  std::smatch match = *cave_begin;
  std::string name = match.str(1);
  auto value = static_cast<Valve::val_t>(std::stoi(match.str(2)));
  std::string outlets = match.str(3);

  auto cave = Cave(std::make_shared<Valve>(name, value));
  return {cave, name, outlets};
}

PipeRunner aoc::steam::from_strings(const std::vector<std::string> & lines){
  std::map<std::string, Cave> caves;
  std::map<std::string, std::string> outlets;
  std::vector<std::string> order;
  order.reserve(lines.size());
 
  for (const auto & line: lines){
    auto [cave, name, out] = cave_from_string(line);
    order.push_back(name);
    caves.emplace(name, cave);
    outlets.emplace(name, out);
  }
  // Sort the caves to ensure we start in cave AA
  std::sort(order.begin(), order.end());

  std::vector<Cave> pr;
  pr.reserve(lines.size());
  for (const auto & name: order){
    auto outs = aoc::line_to_columns(outlets.at(name));
     auto cave = caves.at(name);
     for (const auto & out: outs) {
      cave.add_option(caves.at(out).valve());
    }
     pr.push_back(cave);
  }

  return PipeRunner(std::move(pr));
}

