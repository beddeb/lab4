cmake_minimum_required(VERSION 3.28)
project(lab4)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXE_LINKER_FLAGS "-static -static-libgcc")
set(SFML_STATIC_LIBRARIES TRUE)

add_executable(lab4 main.cpp
        src/graph.tpp)

set(SFML_DIR externallibs/sfml/sfmlx64win/SFML-2.6.1/lib/cmake/SFML)

target_link_libraries(lab4 sfml-graphics)

find_package(SFML 2.6.1 REQUIRED graphics window system)