
enable_testing()
add_executable(bench_compute_vorocell_voro compute_voro.cpp
        treebuild.cpp)
target_link_libraries(bench_compute_vorocell_voro benchmark::benchmark voro++ hashoctree)

add_executable(bench_treebuild treebuild.cpp
        treebuild.cpp)
target_link_libraries(bench_treebuild benchmark::benchmark voro++ hashoctree)

add_executable(bench_nn nn.cpp)
target_link_libraries(bench_nn benchmark::benchmark voro++ hashoctree)