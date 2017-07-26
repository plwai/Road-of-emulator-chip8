cmake_minimum_required (VERSION 3.7.0 FATAL_ERROR)
project (ROEChip8)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# version
set (ROEChip8_VERSION_MAJOR 0)
set (ROEChip8_VERSION_MINOR 1)

# config file
configure_file ( 
		"${PROJECT_SOURCE_DIR}/src/ROEChip8Config.h.in" 
		"${PROJECT_BINARY_DIR/ROEChip8Config.h}"
	)

include_directories("${PROJECT_BINARY_DIR}")

add_executable(ROEChip8 src/ROEChip8/ROEChip8.cc)

install (TARGETS ROEChip8 DESTINATION bin)
install (FILES "${PROJECT_BINARY_DIR}/ROEChip8Config.h" DESTINATION include)