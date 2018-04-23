cmake_minimum_required(VERSION 3.5.1)

project(client)

set(CMAKE_CXX_STANDARD 11)

include_directories(src)

file(GLOB SOURCE "src/*.cpp")

add_executable(${PROJECT_NAME} ${SOURCE})
