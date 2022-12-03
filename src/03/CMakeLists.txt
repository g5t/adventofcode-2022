cmake_minimum_required(VERSION 3.22.0)
project(aoc22.03)

set(SOURCES main.cpp)

add_executable(aoc22.03 ${SOURCES})

target_compile_options(aoc22.03 PRIVATE ${AOC22_CXX_FLAGS})

target_link_libraries(aoc22.03 aoc)

install(TARGETS aoc22.03 DESTINATION ${AOC22_INSTALL_BIN})

add_test(NAME test.03.1 COMMAND aoc22.03 ${AOC22_INPUT_DIR}/test.03 1 157)
add_test(NAME test.03.2 COMMAND aoc22.03 ${AOC22_INPUT_DIR}/test.03 2 70)
add_test(NAME aoc22.03.1 COMMAND aoc22.03 ${AOC22_INPUT_DIR}/aoc22.03 1)
add_test(NAME aoc22.03.2 COMMAND aoc22.03 ${AOC22_INPUT_DIR}/aoc22.03 2)
