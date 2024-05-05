add_executable(nn_generate_neighbors generate_neighbor.cpp)
target_link_libraries(nn_generate_neighbors hashoctree)

add_executable(nn_normal_generate_neighbors generate_neigbors_normal.cpp)
target_link_libraries(nn_normal_generate_neighbors hashoctree)

add_executable(nn_normal_neighbors_check neigbors_normal.cpp)
target_link_libraries(nn_normal_neighbors_check hashoctree)


foreach (i RANGE 0 45 5)
    add_executable(knn_accuracy${i} neigbors_accuracy.cpp)
    MATH(EXPR l "${i}+5")
    target_compile_definitions(knn_accuracy${i} PRIVATE FROM_acc=${i} TO_acc=${l})
    target_link_libraries(knn_accuracy${i} hashoctree)
endforeach ()


foreach (i RANGE 0 45 5)
    add_executable(nn_accuracy${i} neigbors_accuracy.cpp)
    MATH(EXPR l "${i}+5")
    target_compile_definitions(nn_accuracy${i} PRIVATE NNTEST FROM_acc=${i} TO_acc=${l})
    target_link_libraries(nn_accuracy${i} hashoctree)
endforeach ()
