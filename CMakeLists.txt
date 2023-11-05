cmake_minimum_required(VERSION 3.26)
project(bp)

set(CMAKE_CXX_STANDARD 20)

option(MERLIN "static link all libs" OFF) # use static linking for glbc/c++ because merging dont have right version for c++20

option(BUILD_TESTS "build tests" OFF)
option(BUILD_BENCHMARK "build benchmark" OFF)
option(BUILD_ALL "build all" OFF)

if (BUILD_ALL)
    set(BUILD_TESTS ON)
    set(BUILD_BENCHMARK ON)
endif ()

include_directories(src)

# add voro lib
add_subdirectory(deps/voro)
include_directories(deps/voro)

add_subdirectory(playground)

if (BUILD_TESTS)
    if (BUILD_BENCHMARK)
        add_subdirectory(deps/googletest)
        include_directories(deps/googletest)
    endif ()
    add_subdirectory(test)
endif ()

if (BUILD_BENCHMARK)
    add_subdirectory(deps/benchmark)
    include_directories(deps/benchmark)
    add_subdirectory(benchmark)
endif ()

if (MERLIN)
    message("merlin")
    set(BUILD_SHARED_LIBS OFF)
    set(CMAKE_EXE_LINKER_FLAGS "-static")

endif ()


#add_compile_options(-fsanitize=address)
#add_link_options(-fsanitize=address)

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
        src/gjk.cpp
)

target_link_libraries(hashoctree voro++)


add_executable(bp
        src/main.cpp
        src/polyhedron.cpp
)

target_link_libraries(bp voro++ hashoctree)



