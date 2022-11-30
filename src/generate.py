#!/bin/env python

def make_cmakelists(root, project, overwrite=False):
  file = root.joinpath("CMakeLists.txt")
  if overwrite or not file.is_file():
    file.touch()
    with open(file, 'w') as f:
      f.write("cmake_minimum_required(VERSION 3.22.0)\n")
      f.write(f"project({project})\n")
      f.write("set(SOURCES main.cpp)\n")
      f.write(f"add_executable({project} ${{SOURCES}})\n")

      f.write(f"target_compile_options({project} PRIVATE ${{AOC22_CXX_FLAGS}})\n")
      
      f.write(f"install(TARGETS {project} DESTINATION ${{AOC22_INSTALL_BIN}})\n")


def make_cpp(root, project, overwrite=False):
  file = root.joinpath("main.cpp")
  if overwrite or not file.is_file():
    file.touch()
    with open(file, 'w') as f:
      f.write("#include <iostream>\n\n")
      f.write("int main(int argc, const char *argv[]){\n")
      f.write("if (argc < 2) { std::cout << \"usage information\"; return 1; }\n")
      f.write("std::cout << argv[0] << \"\\nYour input was:\\n\";")
      f.write("for (int i=1; i<argc; ++i) std::cout << argv[i] << std::endl;\n")
      f.write("return 0;\n")
      f.write("}\n");


def main(root, overwrite=False):
  for d in range(1,26):
    day = f'{d:02d}'
    daydir = root.joinpath(day)
    if not daydir.is_dir():
      daydir.mkdir()

    project = f"aoc22.{day}"
    make_cmakelists(daydir, project, overwrite=overwrite)
    make_cpp(daydir, project, overwrite=overwrite)


if __name__ == '__main__':
  from pathlib import Path
  from argparse import ArgumentParser
  parser = ArgumentParser(prog='generate.py',
    description="Generate or overwrite the skeleton for the 2022 instance of Advent of Code")
  parser.add_argument('-o', '--overwrite', help='Overwrite CMakeLists.txt and main.cpp files', action='store_true')
  args = parser.parse_args()

  root = Path(__file__).resolve().parent

  main(root, overwrite=args.overwrite)
