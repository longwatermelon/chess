project(core)

find_package(SDL2 CONFIG REQUIRED)
find_package(sdl2-image CONFIG REQUIRED)
find_package(sdl2-ttf CONFIG REQUIRED)

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

set(PUBLIC_HEADERS_LIST
	include/core.h
)

source_group("Public" FILES ${PUBLIC_HEADERS_LIST})


target_link_libraries(core PUBLIC SDL2::SDL2 SDL2::SDL2main)
target_link_libraries(core PUBLIC SDL2::SDL2_image)
target_link_libraries(core PUBLIC SDL2::SDL2_ttf)

target_include_directories(core PUBLIC include)
