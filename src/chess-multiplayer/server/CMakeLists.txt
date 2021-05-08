project(server)

find_package(asio CONFIG REQUIRED)

add_executable(server
	src/main.cpp
	src/server.cpp
	src/server.h
)

target_link_libraries(server PRIVATE asio asio::asio)

set_property(TARGET server PROPERTY VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})