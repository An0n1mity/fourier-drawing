cmake_minimum_required(VERSION 3.0)
project(Cycloide C)

set(CMAKE_C_STANDARD 11)

find_package(LibXml2 REQUIRED)
find_package(portaudio REQUIRED)

INCLUDE(FindPkgConfig)
pkg_check_modules(SDL2 REQUIRED sdl2)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)



add_compile_definitions(${LIBXML2_DEFINITIONS} ${GTK3_DEFINITIONS})
add_definitions(${GTK3_CFLAGS_OTHER})
include_directories(${LIBXML2_INCLUDE_DIR} ${SDL2_INCLUDE_DIR} ${GTK3_INCLUDE_DIRS} ${PORTAUDIO_INCLUDE_DIRS})
link_directories(${GTK3_LIBRARY_DIRS})
add_executable(Cycloide main.c shapes.h shapes.c svgparser.h svgparser.c kiss_fft.c kiss_fftr.c pathparser.c fft.c GUI.c audio.c)
target_link_libraries(Cycloide ${LIBXML2_LIBRARIES} ${SDL2_LIBRARIES} ${GTK3_LIBRARIES} portaudio_static m)