cmake_minimum_required(VERSION 3.0)
project(turbo CXX)
set(CMAKE_CXX_STANDARD 14)

add_library(turbo INTERFACE)
target_include_directories(turbo INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include/)

install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/include/turbo_colormap.hpp DESTINATION include)