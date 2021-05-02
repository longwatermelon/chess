project(chess-singleplayer)

add_executable(chess-singleplayer
	src/main.cpp
	src/chess.cpp
	include/chess.h
)

target_link_libraries(chess-singleplayer PUBLIC core)

set_property(TARGET chess-singleplayer PROPERTY VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})