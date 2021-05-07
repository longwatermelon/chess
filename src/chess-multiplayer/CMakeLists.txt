project(chess-multiplayer)

find_path(ASIO_INCLUDE
	NAMES asio.hpp
	HINTS ENV{CMAKE_PREFIX_PATH}
	PATH_SUFFIXES asio-1.18.1/include
)

add_subdirectory(client)
add_subdirectory(server)

target_include_directories(client PRIVATE ${ASIO_INCLUDE})
target_include_directories(server PRIVATE ${ASIO_INCLUDE})