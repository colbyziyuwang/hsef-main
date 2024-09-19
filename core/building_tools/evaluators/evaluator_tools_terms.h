#ifndef EVALUATOR_TOOLS_TERMS_H_
#define EVALUATOR_TOOLS_TERMS_H_

#include <string>

/**
 * Terms to use for logging of the evaluator tools.
 */
namespace evaluatorToolsTerms {
    inline const std::string SETTING_GOAL_TEST = "goal_test";  ///< The goal test used by a NonGoalHeuristic
    inline const std::string SETTING_DEFAULT_H_VALUE = "default_h_value";  ///< The default h-value in the HashMapHeuristic and NonGoalHeuristic (for non-goal nodes)
    inline const std::string SETTING_HASH_FUNCTION = "hash_function";  ///< The hash function used by a HashMapHeuristic
    inline const std::string SETTING_AGGREGATION_OPERATOR = "aggregation_operator";  ///< The name of the set aggration operator used by a SetAggregateEvaluator
    inline const std::string SETTING_SUBEVALUATOR_PREFIX = "subevaluator_";  ///< A subevaluator in a SetAggregateEvaluator
    inline const std::string SETTING_BASE_EVALUATOR = "base_evaluator";  ///< The base evaluator of a DistanceToGoWrapperEvaluator

    inline const std::string OP_NAME_MAX = "max";  ///< Operator label for a max SetAggregateEvaluator
    inline const std::string OP_NAME_SUM = "sum";  ///< Operator label for a sum SetAggregateEvaluator
}  // namespace evaluatorToolsTerms

#endif  //EVALUATOR_TOOLS_TERMS_H_