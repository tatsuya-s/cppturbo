cmake_minimum_required(VERSION 3.0)
project(cppturbo CXX)
set(CMAKE_CXX_STANDARD 14)

add_library(cppturbo INTERFACE)
target_include_directories(cppturbo INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include/)

install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/include/cppturbo.hpp DESTINATION include)