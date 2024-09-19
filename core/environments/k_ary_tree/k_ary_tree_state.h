#ifndef K_ARY_TREE_STATE_H_
#define K_ARY_TREE_STATE_H_

#include <iostream>
#include <vector>

#include "k_ary_tree_action.h"

/**
 * Defines a k-ary tree state.
 * A state is represented by a list of action indices that led to this state.
 * Each action index in the list corresponds to an action taken at a specific node in the k-ary tree.
 * The list of action indices provides a sequence of steps that can be followed to reach the current state.
 *
 * @class KAryTreeState
 */
class KAryTreeState {
public:
    /**
     * Creates an empty state.
     * An empty state is the root of the tree.
     */
    KAryTreeState() = default;

    /**
     * Creates a k-ary tree state from the given list of actions.
     *
     * @param actions The list of actions that led to this state.
     */
    explicit KAryTreeState(const std::vector<KAryTreeAction>& actions);

    std::vector<KAryTreeAction> m_actions;  ///< The list of actions that led to this state.
};

/**
 * Outputs a string representation of the k-ary tree state to the given output stream.
 *
 * @param out The output stream.
 * @param state The k-ary tree state to output.
 * @return The output stream.
 */
std::ostream& operator<<(std::ostream& out, const KAryTreeState& state);

/**
 * Defines equality of two k-ary tree states.
 *
 * @param state1 The first state to test.
 * @param state2 The second state to test.
 * @return If the states are equal or not.
 */
bool operator==(const KAryTreeState& state1, const KAryTreeState& state2);

/**
 * Defines inequality for k-ary tree states.
 *
 * @param state1 The first state to compare.
 * @param state2 The second state to compare.
 * @return If the states are not equal.
 */
bool operator!=(const KAryTreeState& state1, const KAryTreeState& state2);

#endif /* K_ARY_TREE_STATE_H_ */