cmake_minimum_required(VERSION 3.22.0)
project(aoc22.17)

set(SOURCES main.cpp tetrastack.cpp)
add_executable(aoc22.17 ${SOURCES})
target_compile_options(aoc22.17 PRIVATE ${AOC22_CXX_FLAGS})
install(TARGETS aoc22.17 DESTINATION ${AOC22_INSTALL_BIN})
add_test(NAME test.17.1 COMMAND aoc22.17 ${AOC22_INPUT_DIR}/test.17 1 3068)
add_test(NAME test.17.2 COMMAND aoc22.17 ${AOC22_INPUT_DIR}/test.17 2 1514285714288)
add_test(NAME aoc22.17.1 COMMAND aoc22.17 ${AOC22_INPUT_DIR}/aoc22.17 1 3153)
add_test(NAME aoc22.17.2 COMMAND aoc22.17 ${AOC22_INPUT_DIR}/aoc22.17 2)
