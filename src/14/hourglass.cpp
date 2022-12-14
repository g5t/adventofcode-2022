#include "hourglass.h"
#include <regex>

std::vector<std::pair<unsigned, unsigned>> aoc::sand::path_from_string(const std::string& line){
  std::regex pair_regex("([0-9]+),([0-9]+)(\\s->\\s)?");
  auto pair_begin = std::sregex_iterator(line.begin(), line.end(), pair_regex);
  auto pair_end = std::sregex_iterator();
  auto pair_count = std::distance(pair_begin, pair_end);

  std::vector<std::pair<unsigned, unsigned>> pairs;
  pairs.reserve(static_cast<size_t>(pair_count));
  for (auto i = pair_begin; i != pair_end; ++i){
    std::smatch match = *i;
    pairs.emplace_back(std::stoi(match.str(1)), std::stoi(match.str(2)));
  }

  return pairs;
}

aoc::sand::HourGlass aoc::sand::from_strings(const std::vector<std::string>& lines, int part){
  std::vector<std::vector<std::pair<unsigned, unsigned>>> paths;
  paths.reserve(lines.size());
  for (const auto & line: lines) paths.push_back(path_from_string(line));

  std::pair<unsigned, unsigned> xlim, ylim{0,0};
  xlim.first = xlim.second = paths[0][0].first;
  for (const auto & path: paths) for (const auto & p: path) {
    if (p.first < xlim.first) xlim.first = p.first;
    if (p.second < ylim.first) ylim.first = p.second;
    if (p.first > xlim.second) xlim.second = p.first;
    if (p.second > ylim.second) ylim.second = p.second;
  }
  if (part > 1){
    std::vector<std::pair<unsigned, unsigned>> bottom;
    ylim.second += 2;
    if (500 - ylim.second < xlim.first) xlim.first = 500 - ylim.second;
    if (500 + ylim.second > xlim.second) xlim.second = 500 + ylim.second;
    bottom.emplace_back(xlim.first, ylim.second);
    bottom.emplace_back(xlim.second, ylim.second);
    paths.push_back(bottom);
  }

  return {xlim, ylim, paths};
}


std::map<aoc::sand::Material, aoc::bitmap::Color> aoc::sand::material_color_map(){
  using namespace aoc::sand;
  using namespace aoc::bitmap;
  std::map<Material, Color> map;
  map.insert({Material::air, Color(250, 250, 250)});
  map.insert({Material::sand, Color(213, 194, 165)});
  map.insert({Material::stone, Color(133, 138, 126)});
  return map;
}