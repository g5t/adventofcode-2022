cmake_minimum_required(VERSION 3.22.0)
project(aoc22.13)

add_executable(aoc22.13 main.cpp packet.cpp)
target_compile_options(aoc22.13 PRIVATE ${AOC22_CXX_FLAGS})
install(TARGETS aoc22.13 DESTINATION ${AOC22_INSTALL_BIN})
add_test(NAME test.13.1 COMMAND aoc22.13 ${AOC22_INPUT_DIR}/test.13 1 13)
add_test(NAME test.13.2 COMMAND aoc22.13 ${AOC22_INPUT_DIR}/test.13 2 140)
add_test(NAME aoc22.13.1 COMMAND aoc22.13 ${AOC22_INPUT_DIR}/aoc22.13 1)
add_test(NAME aoc22.13.2 COMMAND aoc22.13 ${AOC22_INPUT_DIR}/aoc22.13 2)

