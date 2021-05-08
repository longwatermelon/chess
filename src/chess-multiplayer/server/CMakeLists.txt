project(server)

if (UNIX)
	find_package(asio CONFIG REQUIRED COMPONENTS pthread system)
else()
	find_package(asio CONFIG REQUIRED)
endif()

add_executable(server
	src/main.cpp
	src/server.cpp
	src/server.h
)

target_link_libraries(server PRIVATE asio asio::asio)

target_link_libraries(server PUBLIC core)
set_property(TARGET server PROPERTY VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})