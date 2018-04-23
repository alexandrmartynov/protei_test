cmake_minimum_required(VERSION 3.5.1)

project(server)

set(CMAKE_CXX_STANDARD 11)

file(GLOB SOURCE "*.cpp")

add_executable(${PROJECT_NAME} ${SOURCE})

target_link_libraries(server PUBLIC ioservice)
