cmake_minimum_required(VERSION 3.1)

project(turbo)
set(CMAKE_CXX_STANDARD 14)

include_directories(include)

add_executable(colormap_test src/main.cpp)
target_link_libraries(colormap_test)