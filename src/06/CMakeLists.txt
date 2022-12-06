cmake_minimum_required(VERSION 3.22.0)
project(aoc22.06)

set(SOURCES main.cpp)

add_executable(aoc22.06 ${SOURCES})

target_compile_options(aoc22.06 PRIVATE ${AOC22_CXX_FLAGS})

target_link_libraries(aoc22.06 aoc)

install(TARGETS aoc22.06 DESTINATION ${AOC22_INSTALL_BIN})

add_test(NAME test.06.1.1 COMMAND aoc22.06 ${AOC22_INPUT_DIR}/test.06 1 7)
add_test(NAME test.06.1.2 COMMAND aoc22.06 bvwbjplbgvbhsrlpgdmjqwftvncz 1 5)
add_test(NAME test.06.1.3 COMMAND aoc22.06 nppdvjthqldpwncqszvftbrmjlhg 1 6)
add_test(NAME test.06.1.4 COMMAND aoc22.06 nznrnfrfntjfmvfwmzdfjlvtqnbhcprsg 1 10)
add_test(NAME test.06.1.5 COMMAND aoc22.06 zcfzfwzzqfrljwzlrfnpqdbhtmscgvjw 1 11)
add_test(NAME test.06.2.1 COMMAND aoc22.06 ${AOC22_INPUT_DIR}/test.06 2 19)
add_test(NAME test.06.2.2 COMMAND aoc22.06 bvwbjplbgvbhsrlpgdmjqwftvncz 2 23)
add_test(NAME test.06.2.3 COMMAND aoc22.06 nppdvjthqldpwncqszvftbrmjlhg 2 23)
add_test(NAME test.06.2.4 COMMAND aoc22.06 nznrnfrfntjfmvfwmzdfjlvtqnbhcprsg 2 29)
add_test(NAME test.06.2.5 COMMAND aoc22.06 zcfzfwzzqfrljwzlrfnpqdbhtmscgvjw 2 26)
add_test(NAME aoc22.06.1 COMMAND aoc22.06 ${AOC22_INPUT_DIR}/aoc22.06 1)
add_test(NAME aoc22.06.2 COMMAND aoc22.06 ${AOC22_INPUT_DIR}/aoc22.06 2)
