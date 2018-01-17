find_package(OpenGL REQUIRED)
find_package(GLFW3 3.2 REQUIRED)
find_package(GLM 0.9.9 REQUIRED)
find_package(freetype REQUIRED)
find_library(OpenAL NAMES OpenAL32 HINTS "$ENV{OPENAL_DIR}/libs/Win64")
find_library(LibOgg NAMES libogg_static HINTS "$ENV{LIBOGG_DIR}/win32/lib")
find_library(LibVorbis NAMES libvorbis_static HINTS "$ENV{LIBVORBIS_DIR}/win32/lib")
find_library(LibVorbisFile NAMES libvorbisfile_static HINTS "$ENV{LIBVORBIS_DIR}/win32/lib")

# path
set (OGLSP_ROOT_PATH "${CMAKE_CURRENT_SOURCE_DIR}")
set (OglSP_SOURCE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/src/starter")
set (OglSP_SHADER_PATH "${CMAKE_CURRENT_SOURCE_DIR}/src/shaders")

# config file
configure_file ( 
	"${CMAKE_CURRENT_SOURCE_DIR}/src/StarterConfig.h.in" 
	"${PROJECT_BINARY_DIR}/StarterConfig.h"
)

set ( source_files
	${OglSP_SOURCE_PATH}/Game.cpp
	${OglSP_SOURCE_PATH}/GameImpl.cpp
	${OglSP_SOURCE_PATH}/GameObject.cpp
	${OglSP_SOURCE_PATH}/ResourceManager.cpp
	${OglSP_SOURCE_PATH}/ResourceImpl.cpp

	${OglSP_SOURCE_PATH}/Renderer/Shader.cpp
	${OglSP_SOURCE_PATH}/Renderer/Texture2D.cpp
	${OglSP_SOURCE_PATH}/Renderer/TextRenderer.cpp
	${OglSP_SOURCE_PATH}/Renderer/Renderer2D.cpp
	${OglSP_SOURCE_PATH}/Renderer/SpriteRenderer.cpp
	${OglSP_SOURCE_PATH}/Renderer/FbRenderer.cpp

	${OglSP_SOURCE_PATH}/Audio/AudioEngine.cpp
	${OglSP_SOURCE_PATH}/Audio/Audio.cpp
	${OglSP_SOURCE_PATH}/Audio/OggReader.cpp

	${OglSP_SOURCE_PATH}/CommonUtilities.cpp
)

set ( header_files
	${OglSP_SOURCE_PATH}/Game.h
	${OglSP_SOURCE_PATH}/GameImpl.h
	${OglSP_SOURCE_PATH}/GameObject.h
	${OglSP_SOURCE_PATH}/ResourceManager.h
	${OglSP_SOURCE_PATH}/ResourceImpl.h

	${OglSP_SOURCE_PATH}/Renderer/Shader.h
	${OglSP_SOURCE_PATH}/Renderer/Texture2D.h
	${OglSP_SOURCE_PATH}/Renderer/TextRenderer.h
	${OglSP_SOURCE_PATH}/Renderer/Renderer2D.h
	${OglSP_SOURCE_PATH}/Renderer/SpriteRenderer.h
	${OglSP_SOURCE_PATH}/Renderer/FbRenderer.h

	${OglSP_SOURCE_PATH}/Audio/AudioEngine.h
	${OglSP_SOURCE_PATH}/Audio/Audio.h
	${OglSP_SOURCE_PATH}/Audio/Decoder.h
	${OglSP_SOURCE_PATH}/Audio/OggReader.h

	${OglSP_SOURCE_PATH}/CommonUtilities.h
)

set ( shader_files
	${OglSP_SHADER_PATH}/sprite.vert
	${OglSP_SHADER_PATH}/text.vert

	${OglSP_SHADER_PATH}/sprite.frag
	${OglSP_SHADER_PATH}/text.frag
)

set ( module_source_files
	src/glad/glad.c
)

include_directories("${PROJECT_BINARY_DIR}")

#source_group("Module Source" FILES ${module_source_files})
#source_group("Shader Files" FILES ${shader_files})

add_library(OglSPLib ${source_files} ${header_files} ${module_source_files})

target_include_directories(OglSPLib PUBLIC ${OPENGL_INCLUDE_DIR})
target_include_directories(OglSPLib PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/modules/glad/include")
target_include_directories(OglSPLib PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/modules/stb")
target_include_directories(OglSPLib PUBLIC ${OPENGL_INCLUDE_DIR})
target_include_directories(OglSPLib PUBLIC ${GLM_INCLUDE_DIR})
target_include_directories(OglSPLib PUBLIC ${FREETYPE_INCLUDE_DIRS})
target_include_directories(OglSPLib PUBLIC "$ENV{OPENAL_DIR}/include")
target_include_directories(OglSPLib PUBLIC "$ENV{LIBOGG_DIR}/include")
target_include_directories(OglSPLib PUBLIC "$ENV{LIBVORBIS_DIR}/include")
target_link_libraries(OglSPLib ${FREETYPE_LIBRARIES})
target_link_libraries(OglSPLib ${OPENGL_gl_LIBRARY})
target_link_libraries(OglSPLib glfw)
target_link_libraries(OglSPLib glm)
target_link_libraries(OglSPLib ${OpenAL})
target_link_libraries(OglSPLib ${LibOgg})
target_link_libraries(OglSPLib ${LibVorbis})
target_link_libraries(OglSPLib ${LibVorbisFile})

install (TARGETS OglSPLib DESTINATION lib)