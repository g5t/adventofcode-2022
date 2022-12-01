cmake_minimum_required(VERSION 3.22.0)

project(advent-of-code-2022)

set(CMAKE_C_STANDARD 20)
set(CMAKE_CXX_STANDARD 20)

set(AOC22_CLANG_FLAGS -Werror -pedantic-errors -Wall -Wextra -Wconversion -Wsign-conversion)
set(AOC22_GCC_FLAGS -fanalyzer ${AOC22_CLANG_FLAGS})
set(AOC22_MSVC_FLAGS /WX /W4)

set(AOC22_CXX_FLAGS
	$<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>>:${AOC22_CLANG_FLAGS}>
	$<$<CXX_COMPILER_ID:GNU>:${AOC22_GCC_FLAGS}>
	$<$<CXX_COMPILER_ID:MSVC>:${AOC22_MSVC_FLAGS}>)

set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR})

set(AOC22_LIBRARY_NAME "aoc")
set(AOC22_LIBRARY "aoc")

set(AOC22_INSTALL_INCLUDE ${PROJECT_SOURCE_DIR}/install/include)
set(AOC22_INSTALL_BIN ${PROJECT_SOURCE_DIR}/install/bin)
set(AOC22_INSTALL_LIB ${PROJECT_SOURCE_DIR}/install/lib)

set(AOC22_INPUT_DIR ${PROJECT_SOURCE_DIR}/input)

include_directories(${AOC22_INSTALL_INCLUDE})
include_directories(${AOC22_INSTALL_LIB})

add_subdirectory(lib)

include_directories(lib)

enable_testing()

add_subdirectory(src)

