#ifndef GOAL_TESTS_TOOLS_TERMS_H_
#define GOAL_TESTS_TOOLS_TERMS_H_

#include "building_tools/evaluators/evaluator_tools_terms.h"

#include <string>

/**
 * Terms to use for logging of the goal test tools.
 */
namespace goalTestToolsTerms {
    inline const std::string SETTING_HASH_FUNCTION = evaluatorToolsTerms::SETTING_HASH_FUNCTION;  ///< The hash function used by a MultiStateHashBasedGoalTest
    inline const std::string SETTING_GOAL_STATE = "goal_state";  ///< The goal state used in a SingleGoalTest
}  // namespace goalTestToolsTerms

#endif  //GOAL_TESTS_TOOLS_TERMS_H_