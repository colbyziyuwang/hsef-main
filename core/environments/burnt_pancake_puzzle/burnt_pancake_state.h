#ifndef BURNT_PANCAKE_STATE_H_
#define BURNT_PANCAKE_STATE_H_

#include <iostream>
#include <vector>

using BurntPancake = int;

/**
 * Defines a burnt pancake puzzle state.
 * A burnt pancake puzzle state is a stack of N different sized pancakes, which is represented by
 * a permutation of size N. The integer 1 <= i <= N in this permutation refers to the i-th
 * smallest pancake, and the order of the numbers in the permutation corresponds to the
 * order of the pancakes in the stack from top to bottom. Each pancake is either burnt side up, or not.
 * A positive pancake is right-side up and a negative pancake is upside down.
 * 
 * @class BurntPancakeState
 */
class BurntPancakeState {
public:
    /**
     * Creates a burnt pancake puzzle state of the specified permutation and side-up orientations for each
     * pancake.
     * 
     * @param permutation Vector of size N representing the permutation of the pancake puzzle state.
     * Must contain all integers that have absolute values from 1 to N, where N is the size of the permutation (puzzle).
     * A negative/position integer represents that the pancake is burnt side up/not burnt side up respectively.
     */
    explicit BurntPancakeState(const std::vector<BurntPancake>& permutation);

    std::vector<BurntPancake> m_permutation;  ///< The permutation representation of the state.
};

/**
 * Outputs a string representation of the burnt pancake puzzle state to the given output stream.
 * A '-' before the pancake integer means that it is burnt side up.
 *
 * @param out The output stream.
 * @param state The puzzle to output.
 * @return The output stream.
 */
std::ostream& operator<<(std::ostream& out, const BurntPancakeState& state);

/**
 * Defines equality of two puzzle states.
 *
 * @param state1 The first state to test.
 * @param state2 The second state to test.
 * @return If the states are equal or not.
 */
bool operator==(const BurntPancakeState& state1, const BurntPancakeState& state2);

/**
 * Defines inequality for puzzle states.
 *
 * @param state1 The first state to compare.
 * @param state2 The second state to compare.
 * @return If the states are not equal.
 */
bool operator!=(const BurntPancakeState& state1, const BurntPancakeState& state2);

#endif /* BURNT_PANCAKE_STATE_H_ */
