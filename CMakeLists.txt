cmake_minimum_required(VERSION 3.24)
project(GameEngine)

set(CMAKE_CXX_FLAGS "-std=c++20 -g")
set(CMAKE_CXX_FLAGS_DEBUG "-std=c++20 -Wall -g -O0 -fsanitize=address -fsanitize=undefined")

add_subdirectory(vendor)
add_subdirectory(src)

add_library(Lib SHARED 
    src/objects.cc
    src/parser.cc
    src/renderer.cc
)

target_include_directories(Lib PUBLIC include/)
target_link_libraries(Lib vendor)