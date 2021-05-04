project(client)

add_executable(client
	src/main.cpp
	src/client.cpp
	include/client.h
)

target_link_libraries(client PUBLIC core)
set_property(TARGET client PROPERTY VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})