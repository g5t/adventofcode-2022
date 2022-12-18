cmake_minimum_required(VERSION 3.22.0)
project(aoc22.16)

set(SOURCES main.cpp steam.cpp)
add_executable(aoc22.16 ${SOURCES})
target_compile_options(aoc22.16 PRIVATE ${AOC22_CXX_FLAGS})
target_link_libraries(aoc22.16 aoc)
install(TARGETS aoc22.16 DESTINATION ${AOC22_INSTALL_BIN})
add_test(NAME test.16.1 COMMAND aoc22.16 ${AOC22_INPUT_DIR}/test.16 1 1651)
add_test(NAME test.16.2 COMMAND aoc22.16 ${AOC22_INPUT_DIR}/test.16 2 1707)
add_test(NAME aoc22.16.1 COMMAND aoc22.16 ${AOC22_INPUT_DIR}/aoc22.16 1)

# The runtime for this is ~50 minutes -- There's no reason to repeat it.
#add_test(NAME aoc22.16.2 COMMAND aoc22.16 ${AOC22_INPUT_DIR}/aoc22.16 2 2504)
