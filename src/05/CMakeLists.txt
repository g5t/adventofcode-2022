cmake_minimum_required(VERSION 3.22.0)
project(aoc22.05)

set(SOURCES main.cpp)

add_executable(aoc22.05 ${SOURCES})

target_compile_options(aoc22.05 PRIVATE ${AOC22_CXX_FLAGS})

target_link_libraries(aoc22.05 aoc)

install(TARGETS aoc22.05 DESTINATION ${AOC22_INSTALL_BIN})

add_test(NAME test.05.1 COMMAND aoc22.05 ${AOC22_INPUT_DIR}/test.05 1 CMZ)
add_test(NAME test.05.2 COMMAND aoc22.05 ${AOC22_INPUT_DIR}/test.05 2 MCD)
add_test(NAME aoc22.05.1 COMMAND aoc22.05 ${AOC22_INPUT_DIR}/aoc22.05 1)
add_test(NAME aoc22.05.2 COMMAND aoc22.05 ${AOC22_INPUT_DIR}/aoc22.05 2)
