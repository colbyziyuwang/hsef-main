#ifndef MULTI_STATE_HASH_BASED_GOAL_TEST_
#define MULTI_STATE_HASH_BASED_GOAL_TEST_

#include "building_tools/goal_tests/goal_tests_tools_terms.h"
#include "building_tools/hashing/state_hash_function.h"
#include "logging/logging_terms.h"
#include "logging/search_component_settings.h"
#include "search_basics/goal_test.h"
#include "utils/string_utils.h"

#include <cstddef>
#include <string>
#include <unordered_map>
#include <vector>

/**
 * Defines a multi-goal test function using a hash-based approach.
 * Initialized with a state hash function.
 * It uses an unordered_set to store the hash values of the goals.
 * A goal test involves generating the hash of a state and checking if it is in the set of goals.
 * It assumes all goals have a unique hash value.
 *
 * @class HashBasedMultiGoalTest
 */
template<class State_t, class Hash_t>
class HashBasedMultiGoalTest : public GoalTest<State_t> {
public:
    inline static const std::string CLASS_NAME = "HashBasedMultiGoalTest";  ///< The name of the class. Defines this component's name

    /**
     * Constructs a multi-goal test function with an initial state hash function.
     *
     * @param hash_function The state hash function.
     * @param goal_states The list of goal states
     */
    HashBasedMultiGoalTest(const StateHashFunction<State_t, Hash_t>& hash_function, const std::vector<State_t>& goal_states);

    // Overridden GoalTest functions
    bool isGoal(const State_t& state) const override { return m_goal_hashes.find(m_hash_function->getHashValue(state)) != m_goal_hashes.end(); }

    // Overriden public SettingsLogger methods
    std::string getName() const override { return CLASS_NAME; }

protected:
    // Overriden protected SettingsLogger methods
    StringMap getComponentSettings() const override;
    SearchSettingsMap getSubComponentSettings() const override { return {{goalTestToolsTerms::SETTING_HASH_FUNCTION, m_hash_function->getAllSettings()}}; }

private:
    const StateHashFunction<State_t, Hash_t>* m_hash_function;  ///< The state hash function
    std::vector<State_t> m_goal_states;  ///< The list of goal states
    std::unordered_map<Hash_t, std::size_t> m_goal_hashes;  ///< Set of goal hashes
};

template<class State_t, class Hash_t>
HashBasedMultiGoalTest<State_t, Hash_t>::HashBasedMultiGoalTest(const StateHashFunction<State_t, Hash_t>& hash_function,
          const std::vector<State_t>& goal_states)
          : m_hash_function(&hash_function), m_goal_states(goal_states) {
    assert(hash_function.isPerfectHashFunction());

    for (std::size_t i = 0; i < m_goal_states.size(); i++) {
        m_goal_hashes[m_hash_function->getHashValue(m_goal_states[i])] = i;
    }
}

template<class State_t, class Hash_t>
StringMap HashBasedMultiGoalTest<State_t, Hash_t>::getComponentSettings() const {
    StringMap settings;

    for (std::size_t i = 0; i < m_goal_states.size(); i++) {
        settings[goalTestToolsTerms::SETTING_GOAL_STATE + "_" + std::to_string(i + 1)] = streamableToString(m_goal_states[i]);
    }

    return settings;
}

#endif /* MULTI_STATE_HASH_BASED_GOAL_TEST_ */
