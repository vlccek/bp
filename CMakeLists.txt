cmake_minimum_required(VERSION 3.26)
project(bp)

set(CMAKE_CXX_STANDARD 20)

option(MyOption "merlin" OFF) # use static linking for glbc/c++ because merging dont have right version for c++20


set(CMAKE_CXX_FLAGS_RELEASE "-O3")
#set(CMAKE_CXX_FLAGS_DEBUG  "-Wall -Wextra -pedantic")

include_directories(src)

# add voro lib
add_subdirectory(deps/voro)
include_directories(deps/voro)

add_subdirectory(playground)

find_package(OpenMP)
if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()


# For creating staticly linked (merlin)
#set(BUILD_SHARED_LIBS OFF)
#set(CMAKE_EXE_LINKER_FLAGS "-static")


# add_compile_options(-fsanitize=address)
# add_link_options(-fsanitize=address)

add_library(hashoctree
        src/point.cpp
        src/randompoint.cpp
        src/randompoint.h
        src/hastree.cpp
        src/hastree.h
        src/OctreNodeBuilder.cpp
        src/OctrerNodeBuilder.h
        src/box.cpp
        src/polyhedron.cpp
        src/bkj.cpp
)

target_link_libraries(hashoctree voro++)


add_executable(bp
        src/main.cpp
        src/polyhedron.cpp
)

target_link_libraries(bp voro++ hashoctree)



