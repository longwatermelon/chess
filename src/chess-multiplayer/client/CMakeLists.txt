project(client)

find_package(asio CONFIG REQUIRED)

add_executable(client
	src/main.cpp
	src/client.cpp
	src/client.h
)

target_link_libraries(client PRIVATE asio asio::asio)

target_link_libraries(client PUBLIC core)
set_property(TARGET client PROPERTY VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})