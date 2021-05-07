project(core)

add_library(core
	src/graphics.cpp
	src/piece.cpp
	src/core.cpp
	src/utils.cpp
	src/graphics.h
	src/piece.h
	src/utils.h
	include/core.h
)

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

find_path(SDL2_TTF_INCLUDE
	NAMES SDL_ttf.h
	HINTS ENV{CMAKE_PREFIX_PATH}
	PATH_SUFFIXES SDL2_ttf-2.0.15/include
)

find_library(SDL2_TTF_LIB
	NAMES SDL2_ttf.lib
	HINTS ENV{CMAKE_PREFIX_PATH}
	PATH_SUFFIXES SDL2_ttf-2.0.15/lib/x64
)

target_include_directories(core PUBLIC
	${CMAKE_SOURCE_DIR}
	${SDL2_INCLUDE_DIR}
	${SDL2_IMAGE_INCLUDE}
	${SDL2_TTF_INCLUDE}
	include
)

target_link_libraries(core PUBLIC
	${SDL2_LIB}
	${SDL2MAIN_LIB}
	${SDL2_IMAGE_LIB}
	${SDL2_TTF_LIB}
)
