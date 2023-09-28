cmake_minimum_required(VERSION 3.26)
project(bp)

set(CMAKE_CXX_STANDARD 23)

# add voro lib
add_subdirectory(deps/voro)
find_package(voro++)


add_executable(bp
        main.cpp
        point.cpp
        randompoint.cpp
        randompoint.h
        pointstree.cpp
        pointstree.h
        voronoi.cpp
        voronoi.h
)

target_link_libraries(bp voro++)




