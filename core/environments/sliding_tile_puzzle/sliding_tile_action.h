#ifndef SLIDING_TILE_ACTION_H_
#define SLIDING_TILE_ACTION_H_

#include <cstdint>
#include <ostream>

/**
 * Defines the applicable actions in the sliding tile puzzle domain.
 *
 * Note that left corresponds to "sliding the blank left." The action listed
 * as "dummy" is the dummy action.
 *
 * @class BlankSlide
 */
enum class BlankSlide : std::uint8_t {
    left,
    up,
    down,
    right
};

/**
 * Defines an output representation tile puzzle actions.
 *
 * @param out The output stream.
 * @param action The action to output.
 * @return A string representation of the given action.
 */
std::ostream& operator<<(std::ostream& out, const BlankSlide& action);

#endif  //SLIDING_TILE_ACTION_H_
