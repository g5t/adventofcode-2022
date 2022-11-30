file(GLOB_RECURSE listfiles ${CURRENT_CMAKE_DIR} */CMakeLists.txt)

message(STATUS "List the files")
foreach(X IN LISTS listfiles)
  message(STATUS "${X}")
	cmake_path(GET X PARENT_PATH parent)
	cmake_path(GET parent FILENAME subdir)
	add_subdirectory("${subdir}")
endforeach()
