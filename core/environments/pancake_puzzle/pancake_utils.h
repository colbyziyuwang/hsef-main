#ifndef PANCAKE_UTILS_H_
#define PANCAKE_UTILS_H_

#include "pancake_transitions.h"

#include <ostream>

/**
 * Outputs string representation of the cost_type.
 *
 * @param out The output stream
 * @param cost_type The cost type
 * @return The output stream
 */
std::ostream& operator<<(std::ostream& out, const PancakePuzzleCostType& cost_type);

#endif  //PANCAKE_UTILS_H_
