cmake_minimum_required(VERSION 3.0)
project(cppturbo_examples CXX)
set(CMAKE_CXX_STANDARD 14)

find_package(VTK QUIET)

if (VTK_FOUND)
    include(${VTK_USE_FILE})
    add_executable(zbuffer zbuffer.cpp)
    target_link_libraries(zbuffer ${VTK_LIBRARIES})
    target_include_directories(zbuffer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../include/)
endif()

add_executable(print print.cpp)
target_include_directories(print PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../include/)