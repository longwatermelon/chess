project(server)

add_executable(server
	src/main.cpp
	include/server.h
)

target_link_libraries(server PUBLIC core)
set_property(TARGET server PROPERTY VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})