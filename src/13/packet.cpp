#include "packet.h"

aoc::packet::Packet aoc::packet::from_string(const std::string & line) {
  if (line[0] != '[') throw std::runtime_error("Lists must start with a '['");
  using namespace aoc::packet;

  std::vector<Packet> list;

  int count{0};
  for (size_t i=1; i<line.size();) switch(line[i]){
    case '[': {
      // search for the matching paired ']'
      size_t j = i;
      count = 1;
      while (count && j++ < line.size()) {
        if (line[j] == '[') count++;
        if (line[j] == ']') count--;
      }
      list.push_back(from_string(line.substr(i, j)));
      i = j + 1;
      break;
    }
    case ']': {
      i++; // this *should* be the end of this list
      break;
    }
    default: {
      // find the next comma
      auto idx = line.substr(i).find(',');
      // whatever is between *should* be an integer
      auto extract = idx == std::string::npos ? line.substr(i) : line.substr(i, idx);
      // but might be just ',' which we skip
      if (!extract.empty()) list.emplace_back(std::stoi(extract));
      i = (idx == std::string::npos ? line.size() : i + idx + 1);
    }
  }

	return list;
}


