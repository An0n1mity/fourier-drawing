cmake_minimum_required(VERSION 3.0.0)
project(GTKChess VERSION 0.1.0)

find_package(portaudio REQUIRED)
INCLUDE(FindPkgConfig)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)


add_compile_definitions(${GTK3_DEFINITIONS})
add_definitions(${GTK3_CFLAGS_OTHER})
include_directories(${GTK3_INCLUDE_DIRS} ${PORTAUDIO_INCLUDE_DIRS})
link_directories(${GTK3_LIBRARY_DIRS})
add_executable(GTKChess main.c BoardGUI.c BoardLogic.c PiecesLogic.c Audio.c AI.c TT.c)
target_link_libraries(GTKChess ${GTK3_LIBRARIES} portaudio_static)
