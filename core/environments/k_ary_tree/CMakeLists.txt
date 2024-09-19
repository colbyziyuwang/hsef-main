set(K_ARY_TREE_FILES
    # cmake-format: sortable
    k_ary_tree_action.h k_ary_tree_names.h k_ary_tree_state.cpp k_ary_tree_state.h k_ary_tree_transitions.cpp
    k_ary_tree_transitions.h)

list(TRANSFORM K_ARY_TREE_FILES PREPEND environments/k_ary_tree/)
set(K_ARY_TREE_FILES
    ${K_ARY_TREE_FILES}
    PARENT_SCOPE)
