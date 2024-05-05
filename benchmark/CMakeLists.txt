
enable_testing()
add_executable(bench_compute_vorocell_voro compute_voro.cpp)
target_link_libraries(bench_compute_vorocell_voro benchmark::benchmark voro++ hashoctree)

add_executable(bench_treebuild treebuild.cpp
       # points_3d_normal_plane.cpp
)
target_link_libraries(bench_treebuild benchmark::benchmark voro++ hashoctree)

add_executable(bench_knn knn.cpp)
target_link_libraries(bench_knn benchmark::benchmark voro++ hashoctree)

add_executable(bench_nn nn.cpp)
target_link_libraries(bench_nn benchmark::benchmark voro++ hashoctree)

add_executable(bench_nnNormal nnNormal.cpp)
target_link_libraries(bench_nnNormal benchmark::benchmark voro++ hashoctree)