#!/bin/env python

def make_cmakelists(root, day, project, overwrite=False):
  file = root.joinpath("CMakeLists.txt")
  if overwrite or not file.is_file():
    file.touch()
    with open(file, 'w') as f:
      f.write("cmake_minimum_required(VERSION 3.22.0)\n")
      f.write(f"project({project})\n\n")
      f.write("set(SOURCES main.cpp)\n")
      f.write(f"add_executable({project} ${{SOURCES}})\n")
      f.write(f"target_compile_options({project} PRIVATE ${{AOC22_CXX_FLAGS}})\n")
      f.write(f"install(TARGETS {project} DESTINATION ${{AOC22_INSTALL_BIN}})\n")

      # write tests first so that trial run last
      for part in (1, 2):
        f.write(f"add_test(NAME test.{day}.{part} COMMAND {project} ${{AOC22_INPUT_DIR}}/test.{day} {part} ###)\n")
      for part in (1, 2):
        f.write(f"#add_test(NAME {project}.{part} COMMAND {project} ${{AOC22_INPUT_DIR}}/{project} {part})\n")


CPP_CONTENTS="""
#include <iostream>
#include <libaoc.h>

int main(int argc, const char *argv[]){
  if (argc > 4 || argc < 3) {
      std::cout << "usage: " << argv[0] << " [filename|input] [part] {[expected result]}" << std::endl;
    return 1;
  }
  using namespace aoc;

  std::string filename = argv[1];
  int part = std::stoi(argv[2]);
  bool do_test = argc > 3;
  int result = (argc > 3) ? std::stoi(argv[3]) : -1;
  int answer = -3;

  auto lines = read_vector_from_lines<NewClass>(filename, NewClassParser);

  if (1 == part){
    answer = do_something_with(lines);
  }

  /* Parse input string or file to produce 'answer' */

  std::cout << (do_test ? "Test " : "Part ") << part << " results in " << answer << "\\n";

  /* Compare the expected result and indicate failure for CTest */
  return (do_test && result != static_cast<int>(answer)) ? 1 : 0;
}
"""


def make_cpp(root, day, project, puzzle_name=None, overwrite=False):
  file = root.joinpath("main.cpp")
  if overwrite or not file.is_file():
    file.touch()
    with open(file, 'w') as f:
      f.write(f"// Advent of Code day {day} -- binary {project}\n")
      if puzzle_name is not None:
        f.write(f"// {puzzle_name}\n")
      f.write(CPP_CONTENTS)


def main(root, overwrite=False, days=None, puzzle_name=None):
  if days is None:
    days = range(1, 26)
  for d in days:
    day = f'{d:02d}'
    daydir = root.joinpath(day)
    if not daydir.is_dir():
      daydir.mkdir()

    project = f"aoc22.{day}"
    make_cmakelists(daydir, day, project, overwrite=overwrite)
    make_cpp(daydir, day, project, puzzle_name=puzzle_name, overwrite=overwrite)


# https://stackoverflow.com/a/23535588
from argparse import Action
class InflateRange(Action):
  def __call__(self, parser, namespace, values, option_string=None):
    from argparse import ArgumentError
    lst = []

    if len(values) == 0:
      return None

    for val in values:
      limits = val.split('-')
      if len(limits) > 2:
        raise ArgumentError(self, "%r is not a range of numbers." % part )
      start = limits[0]
      if start == '': start = '1'
      end = limits[-1]
      try:
        lst.extend(list(range(int(start), int(end)+1)))
      except ValueError:
        raise ArgumentError(self, "%r is not a range of numbers." % part )
      
      setattr(namespace, self.dest, lst)


if __name__ == '__main__':
  from pathlib import Path
  from argparse import ArgumentParser
  parser = ArgumentParser(prog='generate.py',
    description="Generate or overwrite the skeleton for the 2022 instance of Advent of Code")
  parser.add_argument('-o', '--overwrite', help='Overwrite CMakeLists.txt and main.cpp files', action='store_true')
  parser.add_argument('-n', '--name', help='Puzzle name(s)', type=str, default=None) 
  parser.add_argument("day", help='Day or days to write', action=InflateRange, nargs="*")
  args = parser.parse_args()

  root = Path(__file__).resolve().parent

  main(root, overwrite=args.overwrite, days=args.day, puzzle_name=args.name)
