

add_executable(loops_voro
        loops_voro.cpp
)
add_executable(loops_voro_better
        lopps_better.cpp
)

add_executable(ghost_cell
        ghostcell.cpp
)

add_executable(face_voro face_voro.cpp)

target_link_libraries(loops_voro voro++ hashoctree)
target_link_libraries(loops_voro_better voro++ hashoctree)
target_link_libraries(ghost_cell voro++ hashoctree)
target_link_libraries(face_voro voro++ hashoctree)