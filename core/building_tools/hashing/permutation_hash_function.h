#ifndef PERMUTATION_HASH_FUNCTION_H_
#define PERMUTATION_HASH_FUNCTION_H_

#include "building_tools/hashing/state_hash_function.h"
#include "logging/logging_terms.h"
#include "logging/search_component_settings.h"
#include "utils/combinatorics.h"

#include <cassert>
#include <cstdint>
#include <string>


/**
 * A simple hash function that compute non-repeating hashes by overlaying factorial value based on each state 
 * 
 * In the case of the pancake puzzle, the incoming numbers represent different sizes of pancakes. 
 * 
 * In the case of the sliding puzzle, the incoming numbers represent the positions of the different squares
 * 
 * @class PermutationHashFunction
 */
template<class State_t>
class PermutationHashFunction : public StateHashFunction<State_t, uint64_t> {
public:
    inline static const std::string CLASS_NAME = "PermutationHashFunction";  ///< The name of the class. Defines this component's name

    uint64_t getHashValue(const State_t& state) const override;
    bool isPerfectHashFunction() const override { return true; };

    // Overriden public SettingsLogger methods
    std::string getName() const override { return CLASS_NAME; }

protected:
    // Overriden protected SettingsLogger methods
    StringMap getComponentSettings() const override { return {}; };
    SearchSettingsMap getSubComponentSettings() const override { return {}; }
};

template<class State_t>
uint64_t PermutationHashFunction<State_t>::getHashValue(const State_t& state) const {
    assert(state.m_permutation.size() <= 20);
    return getPermutationRank(state.m_permutation);
}

#endif  //PERMUTATION_HASH_FUNCTION_H_
