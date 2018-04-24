cmake_minimum_required(VERSION 3.5.1)

set(CMAKE_CXX_STANDARD 11)

file(GLOB SOURCE "*.cpp")

add_library(ioservice STATIC ${SOURCE})

target_include_directories(ioservice PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
