set(EVAL_FUNCTIONS_FILES # cmake-format: sortable
                         eval_function_terms.h f_cost_evaluator.h g_cost_evaluator.h weighted_f_cost_evaluator.h)

list(TRANSFORM EVAL_FUNCTIONS_FILES PREPEND engines/engine_components/eval_functions/)

set(EVAL_FUNCTIONS_FILES
    ${EVAL_FUNCTIONS_FILES}
    PARENT_SCOPE)
