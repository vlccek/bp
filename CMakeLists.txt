cmake_minimum_required(VERSION 3.26)
project(bp)

set(CMAKE_CXX_STANDARD 20)

option(MERLIN "static link all libs" OFF) # use static linking for glbc/c++ because merging dont have right version for c++20

include_directories(src)

# add voro lib
add_subdirectory(deps/voro)
include_directories(deps/voro)

add_subdirectory(playground)


add_subdirectory(deps/googletest)
include_directories(deps/googletest)
add_subdirectory(test)


add_subdirectory(deps/benchmark)
include_directories(deps/benchmark)
add_subdirectory(benchmark)

if (MERLIN)
    message("merlin")
    set(BUILD_SHARED_LIBS OFF)
    set(CMAKE_EXE_LINKER_FLAGS "-static")

endif ()


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
        src/gjk.cpp
)

target_link_libraries(hashoctree voro++)


add_executable(bp
        src/main.cpp
        src/polyhedron.cpp
)

target_link_libraries(bp voro++ hashoctree)



