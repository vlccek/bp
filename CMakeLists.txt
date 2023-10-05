cmake_minimum_required(VERSION 3.26)
project(bp)

set(CMAKE_CXX_STANDARD 23)

option(MyOption "merlin" OFF) # use static linking for glbc/c++ because merging dont have right version for c++20


set(CMAKE_CXX_FLAGS_RELEASE "-O3")
#set(CMAKE_CXX_FLAGS_DEBUG  "-Wall -Wextra -pedantic")

# add voro lib
add_subdirectory(deps/voro)
find_package(voro++)
include_directories(deps/voro)


# For creating staticly linked (merlin)
#set(BUILD_SHARED_LIBS OFF)
#set(CMAKE_EXE_LINKER_FLAGS "-static")


# add_compile_options(-fsanitize=address)
# add_link_options(-fsanitize=address)

add_executable(bp
        src/main.cpp
        src/point.cpp
        src/randompoint.cpp
        src/randompoint.h
        src/hastree.cpp
        src/hastree.h
)

add_executable(test_how_works_loops_in_woro++
        playground/test_how_works_loops_in_woro++.cpp
        src/randompoint.cpp
        src/point.cpp
        src/hastree.cpp)


add_executable(test_how_volumes_works
        playground/test_voronoi_cell_volume.cpp
        src/randompoint.cpp
        src/point.cpp
        src/hastree.cpp)



target_link_libraries(bp voro++)
target_link_libraries(test_how_works_loops_in_woro++ voro++)
target_link_libraries(test_how_volumes_works voro++)




