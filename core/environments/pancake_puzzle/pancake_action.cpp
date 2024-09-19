#include "pancake_action.h"

#include <ostream>
#include <string>

std::ostream& operator<<(std::ostream& out, const NumToFlip& action) {
    out << "f" << std::to_string(action);
    return out;
}