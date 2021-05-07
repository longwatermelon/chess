project(chess-singleplayer)

add_executable(chess-singleplayer
	src/main.cpp
	src/chess.cpp
	src/chess.h
)

target_link_libraries(chess-singleplayer PUBLIC core)

set_property(TARGET chess-singleplayer PROPERTY VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})