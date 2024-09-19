#ifndef STATE_STRING_HASH_FUNCTION_H_
#define STATE_STRING_HASH_FUNCTION_H_

#include <sstream>
#include <string>

#include "building_tools/hashing/state_hash_function.h"

/**
 * A simple hash function that uses the string representation of the state as the hash value. Will not be very efficient
 * but can be used for quick protecting. Assumes that each state has a unique string representation.
 *
 * @class StateStringHashFunction
 */
template<class State_t>
class StateStringHashFunction : public StateHashFunction<State_t, std::string> {
public:
    inline static const std::string CLASS_NAME = "StateStringHashFunction";  ///< The name of the class. Defines this component's name

    // Overriden HashFunction methods
    std::string getHashValue(const State_t& state) const override;
    bool isPerfectHashFunction() const override { return true; }

    // Overriden public SettingsLogger methods
    std::string getName() const override { return CLASS_NAME; }

protected:
    // Overriden protected SettingsLogger methods
    StringMap getComponentSettings() const override { return {}; };
    SearchSettingsMap getSubComponentSettings() const override { return {}; }
};

template<class State_t>
inline std::string StateStringHashFunction<State_t>::getHashValue(const State_t& state) const {
    std::stringstream state_ss;
    state_ss << state;
    return state_ss.str();
}

#endif  //STATE_STRING_HASH_FUNCTION_H_
