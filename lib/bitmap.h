//
// Created by gst on 14/12/22.
//

#ifndef ADVENT_OF_CODE_2022_BITMAP_H
#define ADVENT_OF_CODE_2022_BITMAP_H

#include <stdio.h>
#include <string>
#include <vector>
#include <map>

namespace aoc::bitmap {
  class Color {
  public:
    using t = uint8_t;
  private:
    t r_=0;
    t g_=0;
    t b_=0;
  public:
    Color(t r, t g, t b): r_(r), g_(g), b_(b) {}
    [[nodiscard]] t r() const {return r_;}
    [[nodiscard]] t g() const {return g_;}
    [[nodiscard]] t b() const {return b_;}
    [[nodiscard]] const char * R() const {return (const char *)&r_;}
    [[nodiscard]] const char * B() const {return (const char *)&b_;}
    [[nodiscard]] const char * G() const {return (const char *)&g_;}
    void write(std::ofstream& of) const {
      of.write((char*)&b_, sizeof(uint8_t));
      of.write((char*)&g_, sizeof(uint8_t));
      of.write((char*)&r_, sizeof(uint8_t));
    }
  };
  void write(const std::vector<std::vector<Color>> & image, const std::string & filename);

  template<class T>
  void write(const std::map<T, Color>& map, const std::vector<std::vector<T>>& data, const std::string & filename){
    std::vector<std::vector<Color>> image;
    image.reserve(data.size());
    auto width = data[0].size();
    for (const auto & row: data) if (row.size() > width) width = row.size();

    for (const auto & row: data){
      std::vector<Color> values(width, Color(0,0,0));
      for (size_t i=0; i<row.size(); ++i) values[i] = map.at(row[i]);
      image.push_back(values);
    }

    write(image, filename);
  }

} // namespace aoc::bitmap
#endif //ADVENT_OF_CODE_2022_BITMAP_H
