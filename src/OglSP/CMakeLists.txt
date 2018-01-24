cmake_minimum_required (VERSION 3.7.0 FATAL_ERROR)
project (OglSPLib)

find_package(OpenGL REQUIRED)
find_package(GLFW3 3.2 REQUIRED)
find_package(GLM 0.9.9 REQUIRED)
find_package(freetype REQUIRED)
find_library(OpenAL NAMES OpenAL32 HINTS "$ENV{OPENAL_DIR}/libs/Win64")
find_library(LibOgg NAMES libogg_static HINTS "$ENV{LIBOGG_DIR}/win32/lib")
find_library(LibVorbis NAMES libvorbis_static HINTS "$ENV{LIBVORBIS_DIR}/win32/lib")
find_library(LibVorbisFile NAMES libvorbisfile_static HINTS "$ENV{LIBVORBIS_DIR}/win32/lib")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# version
set (OglSPLib_VERSION_MAJOR 0)
set (OglSPLib_VERSION_MINOR 1)

# path
set (OglSPLib_ROOT_PATH "${CMAKE_CURRENT_SOURCE_DIR}")
set (OglSPLib_SOURCE_PATH "${PROJECT_SOURCE_DIR}/src/starter")
set (OglSPLib_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/include")
set (OglSPLib_SHADER_PATH "${PROJECT_SOURCE_DIR}/src/shaders")

# config file
configure_file ( 
	"${PROJECT_SOURCE_DIR}/src/StarterConfig.h.in" 
	"${PROJECT_BINARY_DIR}/StarterConfig.h"
)

set ( source_files
	${OglSPLib_SOURCE_PATH}/GameObject.cpp
	${OglSPLib_SOURCE_PATH}/ResourceManager.cpp
	${OglSPLib_SOURCE_PATH}/ResourceImpl.cpp

	${OglSPLib_SOURCE_PATH}/Renderer/Shader.cpp
	${OglSPLib_SOURCE_PATH}/Renderer/Texture2D.cpp
	${OglSPLib_SOURCE_PATH}/Renderer/TextRenderer.cpp
	${OglSPLib_SOURCE_PATH}/Renderer/Renderer2D.cpp
	${OglSPLib_SOURCE_PATH}/Renderer/SpriteRenderer.cpp
	${OglSPLib_SOURCE_PATH}/Renderer/FbRenderer.cpp

	${OglSPLib_SOURCE_PATH}/Audio/AudioEngine.cpp
	${OglSPLib_SOURCE_PATH}/Audio/Audio.cpp
	${OglSPLib_SOURCE_PATH}/Audio/OggReader.cpp

	${OglSPLib_SOURCE_PATH}/CommonUtilities.cpp
)

set ( header_files
	${OglSPLib_INCLUDE_DIR}/GameObject.h
	${OglSPLib_INCLUDE_DIR}/ResourceManager.h
	${OglSPLib_INCLUDE_DIR}/ResourceImpl.h

	${OglSPLib_INCLUDE_DIR}/Renderer/Shader.h
	${OglSPLib_INCLUDE_DIR}/Renderer/Texture2D.h
	${OglSPLib_INCLUDE_DIR}/Renderer/TextRenderer.h
	${OglSPLib_INCLUDE_DIR}/Renderer/Renderer2D.h
	${OglSPLib_INCLUDE_DIR}/Renderer/SpriteRenderer.h
	${OglSPLib_INCLUDE_DIR}/Renderer/FbRenderer.h

	${OglSPLib_INCLUDE_DIR}/Audio/AudioEngine.h
	${OglSPLib_INCLUDE_DIR}/Audio/Audio.h
	${OglSPLib_INCLUDE_DIR}/Audio/Decoder.h
	${OglSPLib_INCLUDE_DIR}/Audio/OggReader.h

	${OglSPLib_INCLUDE_DIR}/CommonUtilities.h
)

set ( OglSPLib_shader_files
	${OglSPLib_SHADER_PATH}/sprite.vert
	${OglSPLib_SHADER_PATH}/text.vert

	${OglSPLib_SHADER_PATH}/sprite.frag
	${OglSPLib_SHADER_PATH}/text.frag
)

set ( module_source_files
	src/glad/glad.c
)

include_directories("${PROJECT_BINARY_DIR}")

file (COPY ${OglSPLib_shader_files} DESTINATION ${CMAKE_BINARY_DIR}/resource/shaders)

add_library(OglSPLib ${source_files} ${module_source_files} ${OglSPLib_shader_files})

source_group("Module Source" FILES ${module_source_files})
source_group("Shader Files" FILES ${OglSPLib_shader_files})

target_include_directories(OglSPLib PUBLIC ${OPENGL_INCLUDE_DIR})
target_include_directories(OglSPLib PUBLIC "${PROJECT_SOURCE_DIR}/modules/glad/include")
target_include_directories(OglSPLib PUBLIC "${PROJECT_SOURCE_DIR}/modules/stb")
target_include_directories(OglSPLib PUBLIC ${OPENGL_INCLUDE_DIR})
target_include_directories(OglSPLib PUBLIC ${GLM_INCLUDE_DIR})
target_include_directories(OglSPLib PUBLIC ${FREETYPE_INCLUDE_DIRS})
target_include_directories(OglSPLib PUBLIC "$ENV{OPENAL_DIR}/include")
target_include_directories(OglSPLib PUBLIC "$ENV{LIBOGG_DIR}/include")
target_include_directories(OglSPLib PUBLIC "$ENV{LIBVORBIS_DIR}/include")
target_include_directories(OglSPLib PUBLIC ${OglSPLib_INCLUDE_DIR})
target_link_libraries(OglSPLib ${FREETYPE_LIBRARIES})
target_link_libraries(OglSPLib ${OPENGL_gl_LIBRARY})
target_link_libraries(OglSPLib glfw)
target_link_libraries(OglSPLib glm)
target_link_libraries(OglSPLib ${OpenAL})
target_link_libraries(OglSPLib ${LibOgg})
target_link_libraries(OglSPLib ${LibVorbis})
target_link_libraries(OglSPLib ${LibVorbisFile})

install (TARGETS OglSPLib DESTINATION lib)

install (FILES "${PROJECT_BINARY_DIR}/StarterConfig.h" DESTINATION include)
install (FILES ${header_files} DESTINATION include)
install (FILES ${OglSPLib_shader_files} DESTINATION resource/shaders)