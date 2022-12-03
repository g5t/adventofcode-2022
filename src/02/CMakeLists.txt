cmake_minimum_required(VERSION 3.22.0)
project(aoc22.02)

set(SOURCES main.cpp)

add_executable(aoc22.02 ${SOURCES})

target_compile_options(aoc22.02 PRIVATE ${AOC22_CXX_FLAGS})

target_link_libraries(aoc22.02 aoc)

install(TARGETS aoc22.02 DESTINATION ${AOC22_INSTALL_BIN})

add_test(NAME test.02.1 COMMAND aoc22.02 ${AOC22_INPUT_DIR}/test.02 1 15)
add_test(NAME test.02.2 COMMAND aoc22.02 ${AOC22_INPUT_DIR}/test.02 2 12)
add_test(NAME aoc22.02.1 COMMAND aoc22.02 ${AOC22_INPUT_DIR}/aoc22.02 1)
add_test(NAME aoc22.02.2 COMMAND aoc22.02 ${AOC22_INPUT_DIR}/aoc22.02 2)

