cmake_minimum_required(VERSION 3.0.0)
set(CMAKE_CXX_STANDARD 17)

cmake_policy(SET CMP0091 NEW)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

project(chess)

add_subdirectory(src/core)
add_subdirectory(src/chess-singleplayer)
add_subdirectory(src/chess-multiplayer)