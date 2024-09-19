#ifndef EVAL_FUNCTION_TERMS_H_
#define EVAL_FUNCTION_TERMS_H_

#include <string>

/**
 * Terms for accessing evaluation function settings.
 */
namespace evalFunctionTerms {
    inline const std::string SETTING_HEURISTIC = "heuristic";  ///< The string for a heuristic as part of an evaluation function
    inline const std::string SETTING_HEURISTIC_WEIGHT = "weight";  ///< The weight on a heuristic
}  // namespace evalFunctionTerms

#endif  //EVAL_FUNCTION_TERMS_H_