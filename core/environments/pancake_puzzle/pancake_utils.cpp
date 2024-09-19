#include "pancake_utils.h"
#include "pancake_names.h"
#include "pancake_transitions.h"

#include <ostream>

std::ostream& operator<<(std::ostream& out, const PancakePuzzleCostType& cost_type) {
    switch (cost_type) {
        case PancakePuzzleCostType::unit:
            out << pancakeNames::COST_UNIT;
            break;
        case PancakePuzzleCostType::heavy:
            out << pancakeNames::COST_HEAVY;
            break;
    }
    return out;
}