#ifndef STATE_HASH_FUNCTION_H_
#define STATE_HASH_FUNCTION_H_

#include "logging/settings_logger.h"

/**
 * A class defining a hash function for states. The hash value type should be a type that std::hash already has
 * a built in specialization setup for.
 *
 * @class StateHashFunction
 */
template<class State_t, class Hash_t>
class StateHashFunction : public SettingsLogger {
public:
    /**
     * Gets the hash value for the given state.
     *
     * @param state The state to generate a hash value for
     * @return The hash value for the given state
     */
    virtual Hash_t getHashValue(const State_t& state) const = 0;

    /**
     * Are the hash values for different states guaranteed to be unique.
     *
     * @return Whether the hash function is guaranteed to return unique values.
     */
    virtual bool isPerfectHashFunction() const = 0;
};

#endif  //STATE_HASH_FUNCTION_H_