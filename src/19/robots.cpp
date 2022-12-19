#include <regex>
#include <iostream>
#include <libaoc.h>
#include "robots.h"

using namespace aoc::robots;

std::shared_ptr<Blueprint> aoc::robots::blueprint_from_string(const std::string & line){
  std::regex rgx(R"(costs ([0-9]+) ore.* ([0-9]+) ore.* ([0-9]+) ore.* ([0-9]+) clay.* ([0-9]+) ore.* ([0-9]+) obsidian)");
	auto begin = std::sregex_iterator(line.begin(), line.end(), rgx);
	auto end = std::sregex_iterator();
	auto count = std::distance(begin, end);
	if (count != 1) {
	  std::string msg = "Regex parsing faile for '";
		msg += line + "'";
		throw std::runtime_error(msg);
	}
	const std::smatch& match = *begin;
	auto ore_ore = static_cast<i_t>(std::stoi(match.str(1)));
	auto clay_ore = static_cast<i_t>(std::stoi(match.str(2)));
	auto obs_ore = static_cast<i_t>(std::stoi(match.str(3)));
	auto obs_clay = static_cast<i_t>(std::stoi(match.str(4)));
	auto geo_ore = static_cast<i_t>(std::stoi(match.str(5)));
	auto geo_obs = static_cast<i_t>(std::stoi(match.str(6)));

	return std::make_shared<Blueprint>(ore_ore, clay_ore, obs_ore, obs_clay, geo_ore, geo_obs);
}

std::ostream & operator<<(std::ostream& os, material m){
  switch(m){
    case material::ore: os << "ore"; return os;
    case material::clay: os << "clay"; return os;
    case material::obsidian: os << "obsidian"; return os;
    case material::geode: os << "geode"; return os;
    case material::none: os << "[nothing]"; return os;
  }
  return os;
}