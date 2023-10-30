enable_testing()

add_executable(test_point test_point.cpp)
add_executable(test_gjk test_gkj.cpp)
add_executable(test_box test_box.cpp)
target_link_libraries(test_point gtest gtest_main hashoctree)
target_link_libraries(test_box gtest gtest_main hashoctree)
target_link_libraries(test_gjk gtest gtest_main hashoctree)