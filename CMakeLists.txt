cmake_minimum_required(VERSION 3.22.0)

project(advent-of-code-2022)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR})

set(AOC22_INSTALL_INCLUDE ${PROJECT_SOURCE_DIR}/include)
set(AOC22_INSTALL_BIN ${PROJECT_SOURCE_DIR}/bin)
set(AOC22_INSTALL_LIB ${PROJECT_SOURCE_DIR}/lib)

include_directories(${AOC_INSTALL_INCLUDE})

add_subdirectory(src)

