cmake_minimum_required(VERSION 3.22.0)
project(aoc22.01)

set(SOURCES main.cpp)

add_executable(aoc22.01 ${SOURCES})

target_compile_options(aoc22.01 PRIVATE ${AOC22_CXX_FLAGS})

target_link_libraries(aoc22.01 aoc)

install(TARGETS aoc22.01 DESTINATION ${AOC22_INSTALL_BIN})

add_test(NAME test.01.1 COMMAND aoc22.01 ${AOC22_INPUT_DIR}/test.01 1 24000)
add_test(NAME test.01.2 COMMAND aoc22.01 ${AOC22_INPUT_DIR}/test.01 2 45000)
add_test(NAME aoc22.01.1 COMMAND aoc22.01 ${AOC22_INPUT_DIR}/aoc22.01 1)
add_test(NAME aoc22.01.2 COMMAND aoc22.01 ${AOC22_INPUT_DIR}/aoc22.01 2)
