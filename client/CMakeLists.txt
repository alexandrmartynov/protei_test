cmake_minimum_required(VERSION 3.5.1)

project(client)

set(CMAKE_CXX_STANDARD 11)

file(GLOB SOURCE "*.cpp")

add_executable(${PROJECT_NAME} ${SOURCE})

target_link_libraries(client PUBLIC ioservice)
