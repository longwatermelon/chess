project(server)

add_executable(server
	src/main.cpp
	src/server.cpp
	src/server.h
)

set_property(TARGET server PROPERTY VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})