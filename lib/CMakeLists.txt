set(SOURCES
    read.cpp
	fakefilesystem.cpp
	bitmap.cpp
	coord.cpp
	)

message(STATUS "Add library ${AOC22_LIBRARY}")
add_library(${AOC22_LIBRARY} SHARED ${SOURCES})

install(TARGETS ${AOC22_LIBRARY} DESTINATION ${AOC22_INSTALL_LIB})
