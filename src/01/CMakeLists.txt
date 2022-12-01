cmake_minimum_required(VERSION 3.22.0)
project(aoc22.01)
set(SOURCES main.cpp)
add_executable(aoc22.01 ${SOURCES})
target_compile_options(aoc22.01 PRIVATE ${AOC22_CXX_FLAGS})
target_link_directories(aoc22.01 PRIVATE ${AOC22_INSTALL_LIB})

target_link_libraries(aoc22.01 PRIVATE ${AOC22_LIBRARY})

install(TARGETS aoc22.01 DESTINATION ${AOC22_INSTALL_BIN})
