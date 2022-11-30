#!/bin/env python

def make_cmakelists(root, project):
  file = root.joinpath("CMakeLists.txt")
  if not file.is_file():
    file.touch()
    with open(file, 'w') as f:
      f.write("cmake_minimum_required(VERSION 3.22.0)\n")
      f.write(f"project({project})\n")
      f.write("set(SOURCES main.cpp)\n")
      f.write(f"add_executable({project} ${{SOURCES}})\n")
      
      f.write(f"install(TARGETS {project} DESTINATION ${{AOC22_INSTALL_BIN}})\n")


def make_cpp(root, project):
  file = root.joinpath("main.cpp")
  if not file.is_file():
    file.touch()
    with open(file, 'w') as f:
      f.write("#include <iostream>\n\n")
      f.write("int main(int argc, const char *argv[]){\n")
      f.write("if (argc < 2) { std::cout << \"usage information\"; return 1; }\n")
      f.write("return 0;\n")
      f.write("}\n");


def main(root):
  for d in range(1,26):
    day = f'{d:02d}'
    daydir = root.joinpath(day)
    if not daydir.is_dir():
      daydir.mkdir()

    project = f"aoc22.{day}"
    make_cmakelists(daydir, project)
    make_cpp(daydir, project)


if __name__ == '__main__':
  from pathlib import Path
  root = Path(__file__).resolve().parent

  main(root)
