enable_testing()

add_executable(test_point test_point.cpp)
add_executable(test_gjk test_gjk.cpp)
add_executable(test_box test_box.cpp)
add_executable(test_nn nn_test.cpp)
add_executable(findbeloginginterval findbelogingInterval.cpp)
add_executable(tree_test test_tree.cpp)
target_link_libraries(test_point gtest gtest_main hashoctree)
target_link_libraries(test_box gtest gtest_main hashoctree)
target_link_libraries(test_gjk gtest gtest_main hashoctree)
target_link_libraries(test_nn gtest gtest_main hashoctree )
# target_link_libraries(test_nn gtest gtest_main hashoctree -static-libgcc -static-libstdc++ -static)
target_link_libraries(findbeloginginterval gtest gtest_main hashoctree)
target_link_libraries(tree_test gtest gtest_main hashoctree)

add_test(NAME test_point COMMAND test_point)
add_test(NAME test_box COMMAND test_box)
add_test(NAME test_nn COMMAND test_nn)
add_test(NAME gjk_test COMMAND test_gjk)


file(GLOB_RECURSE sources
        ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

add_executable(alltests
        ${sources}
)

target_link_libraries(alltests gtest gtest_main hashoctree)