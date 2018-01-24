cmake_minimum_required (VERSION 3.7.0 FATAL_ERROR)
project (ROEChip8)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# version
set (ROEChip8_VERSION_MAJOR 0)
set (ROEChip8_VERSION_MINOR 1)

# path
set (ROOT_PATH "${PROJECT_SOURCE_DIR}")
set (ROECHIP8_SOURCE_PATH "${PROJECT_SOURCE_DIR}/src/ROEChip8")
set (GRAPHIC_OGL_PATH "${PROJECT_SOURCE_DIR}/src/GraphicEngine")

# config file
configure_file ( 
	"${PROJECT_SOURCE_DIR}/src/ROEChip8Config.h.in" 
	"${PROJECT_BINARY_DIR}/ROEChip8Config.h"
)

set ( source_files
	src/main.cpp

	${ROECHIP8_SOURCE_PATH}/ROEChip8.cpp
)

set ( header_files 
	${ROECHIP8_SOURCE_PATH}/ROEChip8.h
)

set ( Ogl_files
	${GRAPHIC_OGL_PATH}/Game.h
	${GRAPHIC_OGL_PATH}/GameImpl.h
	
	${GRAPHIC_OGL_PATH}/Game.cpp
	${GRAPHIC_OGL_PATH}/GameImpl.cpp
)

include_directories("${PROJECT_BINARY_DIR}")

source_group("Graphics (OpenGL)" FILES ${Ogl_files})

add_executable(ROEChip8 ${source_files} ${header_files} ${Ogl_files})

add_subdirectory(src/OglSP)
target_include_directories(ROEChip8 PUBLIC "${PROJECT_SOURCE_DIR}/src/OglSP/include")
target_link_libraries(ROEChip8 OglSPLib)

install (TARGETS ROEChip8 DESTINATION bin)
install (FILES "${PROJECT_BINARY_DIR}/ROEChip8Config.h" DESTINATION include)