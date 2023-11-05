
enable_testing()
add_executable(compute_vorocell compute_voro.cpp)
target_link_libraries(compute_vorocell benchmark::benchmark voro++ hashoctree)

# add_executable(treebuild treebuild.cpp)
# target_link_libraries(treebuild benchmark::benchmark voro++ hashoctree)

add_executable(nn nn.cpp)
target_link_libraries(nn benchmark::benchmark voro++ hashoctree)