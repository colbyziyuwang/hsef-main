#include "sliding_tile_action.h"
#include "sliding_tile_names.h"

#include <ostream>

std::ostream& operator<<(std::ostream& out, const BlankSlide& action) {
    using namespace slidingTileNames;
    if (action == BlankSlide::up) {
        out << DIRECTION_UP;
    } else if (action == BlankSlide::right) {
        out << DIRECTION_RIGHT;
    } else if (action == BlankSlide::down) {
        out << DIRECTION_DOWN;
    } else if (action == BlankSlide::left) {
        out << DIRECTION_LEFT;
    }
    return out;
}
