project(client)

add_executable(client
	src/main.cpp
	include/client.h
)

target_link_libraries(client PUBLIC core)
set_property(TARGET chess-singleplayer PROPERTY VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})