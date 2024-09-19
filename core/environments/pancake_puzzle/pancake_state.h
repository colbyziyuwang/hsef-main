#ifndef PANCAKE_STATE_H_
#define PANCAKE_STATE_H_

#include <iostream>
#include <vector>

using Pancake = int;  ///< The type of a pancake in a permtuation

/**
 * Defines a pancake puzzle state.
 * A pancake puzzle state is a stack of N different sized pancakes, which is represented by
 * a permutation of size N. The integer 0 <= i <= N - 1 in this permutation refers to the i-th
 * smallest pancake, and the order of the numbers in the permutation corresponds to the
 * order of the pancakes in the stack from top to bottom.
 * 
 * @class PancakeState
 */
class PancakeState {
public:
    /**
     * Creates a pancake puzzle state of the specified permutation.
     * 
     * @param permutation Vector of size N representing the permutation of the pancake puzzle state.
     * Must contain all integers from 0 to N - 1, where N is the size of the permutation (puzzle).
     */
    explicit PancakeState(const std::vector<Pancake>& permutation);

    std::vector<Pancake> m_permutation;  ///< The permutation representation of the state.
};

/**
 * Outputs a string representation of the pancake puzzle state to the given output stream.
 *
 * @param out The output stream.
 * @param state The puzzle to output.
 * @return The output stream.
 */
std::ostream& operator<<(std::ostream& out, const PancakeState& state);

/**
 * Defines equality of two puzzle states.
 *
 * @param state1 The first state to test.
 * @param state2 The second state to test.
 * @return If the states are equal or not.
 */
bool operator==(const PancakeState& state1, const PancakeState& state2);

/**
 * Defines inequality for puzzle states.
 *
 * @param state1 The first state to compare.
 * @param state2 The second state to compare.
 * @return If the states are not equal.
 */
bool operator!=(const PancakeState& state1, const PancakeState& state2);

#endif /* PANCAKE_STATE_H_ */
