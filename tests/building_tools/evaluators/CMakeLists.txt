add_standard_test(evaluation_cache_test.cpp)

add_test_with_libs(constant_heuristic_test.cpp TestHelpersLib)
add_test_with_libs(distance_to_go_wrapper_evaluator_test.cpp TestHelpersLib)
add_test_with_libs(set_aggregate_evaluator_factory_test.cpp TestHelpersLib)
add_test_with_libs(hash_map_heuristic_test.cpp TestHelpersLib)
add_test_with_libs(node_evaluator_with_cache_test.cpp TestHelpersLib)
add_test_with_libs(non_goal_heuristic_test.cpp TestHelpersLib)
add_test_with_libs(set_aggregate_evaluator_test.cpp TestHelpersLib)
