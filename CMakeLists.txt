cmake_minimum_required(VERSION 3.5.1)

set(CMAKE_CXX_STANDARD 11)

add_definitions(-Wall)

set(CMAKE_CXX_STANDARD)
set(CMAKE_CXX_STANDARD_REQUIRED on)

add_subdirectory(client)
add_subdirectory(server)
add_subdirectory(common)
