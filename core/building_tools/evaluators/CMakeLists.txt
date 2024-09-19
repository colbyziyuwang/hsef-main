set(EVALUATOR_BUILDING_FILES
    # cmake-format: sortable
    constant_heuristic.h
    cost_and_distance_to_go_evaluator.h
    distance_to_go_wrapper_evaluator.h
    evaluation_cache.cpp
    evaluation_cache.cpp
    evaluator_tools_terms.h
    hash_map_heuristic.h
    node_evaluator_with_cache.h
    non_goal_heuristic.h
    set_aggregate_evaluator.h
    set_aggregate_evaluator_factory.h
    single_goal_state_evaluator.h)

list(TRANSFORM EVALUATOR_BUILDING_FILES PREPEND building_tools/evaluators/)
set(EVALUATOR_BUILDING_FILES
    ${EVALUATOR_BUILDING_FILES}
    PARENT_SCOPE)
