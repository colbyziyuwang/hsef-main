#ifndef SLIDING_TILE_STATE_H_
#define SLIDING_TILE_STATE_H_

#include <iostream>
#include <vector>

using Tile = int;

/**
 * Defines a tile puzzle state.
 *
 * @class SlidingTileState
 */
class SlidingTileState {
public:
    /**
     * Creates an empty tile puzzle with zero in all dimensions.
     */
    SlidingTileState() = default;

    /**
     * Creates a tile puzzle of the given dimensions.
     *
     * @param rows The number of rows in the puzzle.
     * @param cols The number of columns in the puzzle.
     */
    SlidingTileState(int rows, int cols);

    /**
     * Creates a tile puzzle from the given permutation with the given dimensions.
     *
     * @param permutation The permutation as the basis of the puzzle.
     * @param rows The number of rows in the puzzle.
     * @param cols The number of columns in the puzzle.
     */
    SlidingTileState(const std::vector<Tile>& permutation, int rows, int cols);

    std::vector<Tile> m_permutation;  ///< The permutation representation of the state.

    int m_num_rows = 0;  ///< Number of rows in the state.
    int m_num_cols = 0;  ///< Number of columns in the state.

    int m_blank_loc = 0;  ///< Location of the blank (or 0)
};

/**
 * Outputs a string representation of the sliding tile puzzle state to the given output stream.
 *
 * @param out The output stream.
 * @param state The puzzle to output.
 * @return The output stream.
 */
std::ostream& operator<<(std::ostream& out, const SlidingTileState& state);

/**
 * Defines equality of two puzzle states.
 *
 * @param state1 The first state to test.
 * @param state2 The second state to test.
 * @return If the states are equal or not.
 */
bool operator==(const SlidingTileState& state1, const SlidingTileState& state2);

/**
 * Defines inequality for puzzle states.
 *
 * @param state1 The first state to compare.
 * @param state2 The second state to compare.
 * @return If the states are not equal.
 */
bool operator!=(const SlidingTileState& state1, const SlidingTileState& state2);

#endif /* SLIDING_TILE_STATE_H_ */
