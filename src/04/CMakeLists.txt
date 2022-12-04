cmake_minimum_required(VERSION 3.22.0)
project(aoc22.04)

set(SOURCES main.cpp)

add_executable(aoc22.04 ${SOURCES})

target_compile_options(aoc22.04 PRIVATE ${AOC22_CXX_FLAGS})

target_link_libraries(aoc22.04 aoc)

install(TARGETS aoc22.04 DESTINATION ${AOC22_INSTALL_BIN})

add_test(NAME test.04.1 COMMAND aoc22.04 ${AOC22_INPUT_DIR}/test.04 1 2)
add_test(NAME test.04.2 COMMAND aoc22.04 ${AOC22_INPUT_DIR}/test.04 2 4)
add_test(NAME aoc22.04.1 COMMAND aoc22.04 ${AOC22_INPUT_DIR}/aoc22.04 1)
add_test(NAME aoc22.04.2 COMMAND aoc22.04 ${AOC22_INPUT_DIR}/aoc22.04 2)
