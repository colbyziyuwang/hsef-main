#ifndef PANCAKE_ACTIONS_H_
#define PANCAKE_ACTIONS_H_

#include <ostream>

using NumToFlip = int;  ///< The type for a pancake puzzle action

/**
 * Defines an output representation pancake puzzle actions.
 *
 * @param out The output stream.
 * @param action The action to output.
 * @return A string representation of the given action.
 */
std::ostream& operator<<(std::ostream& out, const NumToFlip& action);

#endif  //PANCAKE_ACTIONS_H_
