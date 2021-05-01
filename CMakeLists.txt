cmake_minimum_required(VERSION 3.0.0)
set(CMAKE_CXX_STANDARD 17)

cmake_policy(SET CMP0091 NEW)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

project(chess)

add_executable(chess
	src/main.cpp
	src/graphics.cpp
	src/chess.cpp
	src/piece.cpp
	include/chess.h
	include/graphics.h
	include/piece.h
)

set_property(TARGET chess PROPERTY VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})

find_library(SDL2_LIB
  NAMES SDL2.lib
  HINTS ENV{CMAKE_PREFIX_PATH}
  PATH_SUFFIXES SDL2-2.0.14/lib/x64
)
find_library(SDL2MAIN_LIB
  NAMES SDL2main.lib
  HINTS ENV{CMAKE_PREFIX_PATH}
  PATH_SUFFIXES SDL2-2.0.14/lib/x64
)

find_path(SDL2_INCLUDE_DIR
  NAMES SDL.h
  HINTS ENV{CMAKE_PREFIX_PATH}
  PATH_SUFFIXES SDL2-2.0.14/include
)

find_library(SDL2_IMAGE_LIB
	NAMES SDL2_image.lib
	HINTS ENV{CMAKE_PREFIX_PATH}
	PATH_SUFFIXES SDL2_image-2.0.5/lib/x64
)

find_path(SDL2_IMAGE_INCLUDE
	NAMES SDL_image.h
	HINTS ENV{CMAKE_PREFIX_PATH}
	PATH_SUFFIXES SDL2_image-2.0.5/include
)

include_directories(${SDL2_INCLUDE_DIR} ${SDL2_IMAGE_INCLUDE})
target_link_libraries(chess ${SDL2_LIB} ${SDL2MAIN_LIB} ${SDL2_IMAGE_LIB})
