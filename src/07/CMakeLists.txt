cmake_minimum_required(VERSION 3.22.0)
project(aoc22.07)

set(SOURCES main.cpp)
add_executable(aoc22.07 ${SOURCES})
target_compile_options(aoc22.07 PRIVATE ${AOC22_CXX_FLAGS})
target_link_libraries(aoc22.07  aoc)
install(TARGETS aoc22.07 DESTINATION ${AOC22_INSTALL_BIN})
add_test(NAME test.07.1 COMMAND aoc22.07 ${AOC22_INPUT_DIR}/test.07 1 95437)
add_test(NAME test.07.2 COMMAND aoc22.07 ${AOC22_INPUT_DIR}/test.07 2 24933642)
add_test(NAME aoc22.07.1 COMMAND aoc22.07 ${AOC22_INPUT_DIR}/aoc22.07 1)
add_test(NAME aoc22.07.2 COMMAND aoc22.07 ${AOC22_INPUT_DIR}/aoc22.07 2)
