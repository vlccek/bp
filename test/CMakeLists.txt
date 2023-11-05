enable_testing()

add_executable(test_point test_point.cpp)
add_executable(test_gjk test_gkj.cpp)
add_executable(test_box test_box.cpp)
add_executable(nn_test nn_test.cpp)
target_link_libraries(test_point gtest gtest_main hashoctree)
target_link_libraries(test_box gtest gtest_main hashoctree)
target_link_libraries(test_gjk gtest gtest_main hashoctree)
target_link_libraries(nn_test gtest gtest_main hashoctree)

add_test(NAME test_point COMMAND test_point)
add_test(NAME test_box COMMAND test_box)
add_test(NAME test_gjk COMMAND test_gjk)
add_test(NAME nn_test COMMAND nn_test)


file(GLOB_RECURSE sources
        ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

add_executable(alltests
        ${sources}
)

target_link_libraries(alltests gtest gtest_main hashoctree)