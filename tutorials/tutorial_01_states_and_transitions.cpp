#include "environments/sliding_tile_puzzle/sliding_tile_action.h"
#include "environments/sliding_tile_puzzle/sliding_tile_state.h"
#include "environments/sliding_tile_puzzle/sliding_tile_transitions.h"
#include "utils/string_utils.h"

#include <iostream>
#include <optional>

using std::cout;

int main() {
    // READ THROUGH BOTH THE COMMENTS AND CODE BELOW

    cout << "\n------ TUTORIAL OBJECTIVES ------\n";
    cout << "When creating a system to solve state-space search we first need to define a state and action representation,\n";
    cout << "as well as a \"transition system\" which defines when actions are applicable and the cost of applying them.\n";
    cout << "In this tutorial, you will walk through the use of states and transition systems in HSEF\n";
    cout << "We will do so using the sliding tile puzzle\n";

    cout << "\n\n------ PART 1: Creating States and Testing Equality ------\n";
    // In HSEF, states and actions are objects. For example, see
    // core/environments/sliding_tile_puzzle/sliding_tile_puzzle_state for the state definition.

    // Let's now create several 3x3 sliding tile puzzle states.
    // Notice that the first and third are the same state

    SlidingTileState state_1({1, 2, 3, 4, 5, 8, 7, 0, 6}, 3, 3);
    SlidingTileState state_2({3, 1, 2, 4, 5, 8, 7, 0, 6}, 3, 3);
    SlidingTileState state_3({1, 2, 3, 4, 5, 8, 7, 0, 6}, 3, 3);

    // The output operator has been created for all states
    cout << "tile_state_1: " << state_1 << "\n";
    cout << "tile_state_2: " << state_2 << "\n";
    cout << "tile_state_2: " << state_3 << "\n\n";

    // Equality testing is also defined for such tests
    cout << "tile_state_1 == tile_state_2 : " << boolToString(state_1 == state_2) << "\n";
    cout << "tile_state_1 == tile_state_3 : " << boolToString(state_1 == state_3) << "\n\n";

    //
    //
    cout << "\n\n------ PART 2: Creating Actions and Testing Equality ------\n";
    // For an action definition, see core/environments/sliding_tile_puzzle/sliding_tile_puzzle_action
    // Notice that for simplicity, we model the problem as a blank sliding as opposed to a slide of a
    // tile into blank location. This is merely for convenience
    // Given its simplicity, BlankSlide has been implemented as an enum class
    BlankSlide action_1 = BlankSlide::up;
    BlankSlide action_2 = BlankSlide::left;
    BlankSlide action_3 = BlankSlide::up;

    // Again, the output and equality operators have been defined for actions
    cout << "action_1: " << action_1 << "\n";
    cout << "action_2: " << action_2 << "\n";
    cout << "action_2: " << action_3 << "\n\n";

    cout << "action_1 == action_2 : " << boolToString(action_1 == action_2) << "\n";
    cout << "action_1 == action_3 : " << boolToString(action_1 == action_3) << "\n\n";

    //
    //
    cout << "\n\n------ PART 3: Transition Systems ------\n";
    // A "transition system" now defines how states can change due to actions
    // The generic transition system abstract class is defined in "core/search_basics/transition_system.h"
    // Any transition system that follows this interface will then be able to be used by any search engine (more on
    // this later). Notice that the transition system uses templates, meaning we can create one for any state and
    // action representation.
    //
    // For the sliding tile puzzle, the transition system is defined in
    // "core/environments/sliding_tile_puzzle/sliding_tile_transitions.h"
    // We can create one as follows
    SlidingTileTransitions transitions(3, 3, SlidingTileCostType::unit);

    // Here, we are stating that the size of the puzzle is 3x3 and the cost and we are
    // using the unit-cost variant of the problem (see sliding_tile_transitions.h for the other options)
    // Transition systems now have a variety of available functions.

    // getActions returns a vector of the actions applicable in the given state
    cout << "Actions applicable in state_1: " << vectorToString(transitions.getActions(state_1)) << "\n";
    cout << "Actions applicable in state_2: " << vectorToString(transitions.getActions(state_1)) << "\n\n";

    // isApplicable checks if a given action is applicable in the given statee
    cout << "Is \"up\" applicable in state_1: " << transitions.isApplicable(state_1, BlankSlide::up) << "\n";
    cout << "Is \"down\" applicable in state_1: " << transitions.isApplicable(state_1, BlankSlide::down) << "\n\n";

    // getActionCost returns the cost of applying the given action in the given state.
    // It assumes the action is applicable
    cout << "Cost of applying \"up\" in state_1" << transitions.getActionCost(state_1, BlankSlide::up) << "\n\n";

    // applyAction applies the action to the given state. This will change the given state
    // getChildState acts similarly, but doesn't affect the underlying state
    cout << "state_1 before applying action: " << state_1 << "\n";
    transitions.applyAction(state_1, BlankSlide::up);
    cout << "state_1 after applying \"up\": " << state_1 << "\n\n";

    cout << "state_3 before calling getChildState: " << state_3 << "\n";
    cout << "child state from applying \"up\" in state_3: " << transitions.getChildState(state_3, BlankSlide::up) << "\n";
    cout << "state_3 after calling getChildState: " << state_3 << "\n";

    // Transition systems may also be able to detect actions that have inverses. Action a1 is an inverse of a2 in state s
    // if applying a1 in the state that is the result of applying a2 to s is just s itself.
    // For engines that may use this functionality, we have therefore defined the following methods
    //
    // isInverse checks if an action is the inverse of another in a given state
    cout << R"(is "up" the inverse of "down" in state_3: )" << boolToString(transitions.isInverseAction(state_3, BlankSlide::up, BlankSlide::down)) << "\n";
    cout << R"(is "up" the inverse of "left" in state_3: )" << boolToString(transitions.isInverseAction(state_3, BlankSlide::up, BlankSlide::left)) << "\n\n";

    // getInverse returns the inverse of an action. The return type is an "optional", meaning std::nullopt is returned
    // if the action has no inverse (or inverse checking is not implemented)
    std::optional<BlankSlide> inverse = transitions.getInverse(state_3, BlankSlide::up);
    cout << "\"up\" has a valid inverse in state_3: " << boolToString(inverse.has_value()) << "\n";
    if (inverse.has_value()) {
        cout << "the inverse of \"up\" in state_3: " << inverse.value() << "\n";
    }

    //
    //
    cout << "\n\n------ SUMMARY ------\n";
    cout << " - HSEF enables the creating, comparing, and outputting of states and actions\n";
    cout << " - States and actions are manipulated using transition systems, which define action applicability\n";
    cout << "   and the outcome of applying actions\n";

    return 0;
}