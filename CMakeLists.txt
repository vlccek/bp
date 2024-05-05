cmake_minimum_required(VERSION 3.22)
project(bp)

set(CMAKE_CXX_STANDARD 20)

option(MERLIN "static link all libs" OFF)


if (CMAKE_BUILD_TYPE MATCHES Release)
    message("Release build.")
    SET(CMAKE_CXX_FLAGS "-Ofast -flto=24 -DNDEBUG -std=c++20 -march=native -fpic -ftree-vectorize -ffast-math -pipe ${CMAKE_CXX_FLAGS}")
endif ()


# if type debug add this flag
IF (CMAKE_BUILD_TYPE MATCHES Debug)
    SET(CMAKE_CXX_FLAGS "-march=native ${CMAKE_CXX_FLAGS}")
ENDIF ()


find_package(OpenMP)
if (OPENMP_FOUND)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif ()

# add_compile_options(-fsanitize=address)
# add_link_options(-fsanitize=address)


include_directories(src)

# add voro lib
add_definitions(-Wno-deprecated-declarations)
add_subdirectory(deps/voro)
include_directories(deps/voro/src)
# target_compile_definitions(voro++ PRIVATE " -w")
target_compile_options(voro++ PRIVATE -w)

add_subdirectory(playground)

add_subdirectory(accuracy_tests)


add_subdirectory(deps/googletest)
include_directories(deps/googletest)
add_subdirectory(test)


add_subdirectory(deps/benchmark)
include_directories(deps/benchmark)
add_subdirectory(benchmark)


add_library(hashoctree
        src/point.cpp
        src/randompoint.cpp
        src/randompoint.h
        src/hastree.cpp
        src/hastree.h
        src/OctreeNode.cpp
        src/OctreeNode.h
        src/box.cpp
        src/polyhedron.cpp
        src/gjk.cpp
)

target_link_libraries(hashoctree voro++)


add_executable(main
        src/main.cpp
        src/polyhedron.cpp
)

target_link_libraries(main voro++ hashoctree)



