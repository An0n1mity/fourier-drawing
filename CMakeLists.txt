cmake_minimum_required(VERSION 3.17)
project(svg_parser C)

set(CMAKE_C_STANDARD 99)

add_executable(svg_parser main.c shapes.c svgparser.c svg.h svgparser.h)

if(EXISTS ${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    conan_basic_setup()
else()
    message(WARNING "The file conanbuildinfo.cmake doesn't exist, you have to run conan install first")
endif()
