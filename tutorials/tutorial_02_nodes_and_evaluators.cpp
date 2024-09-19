#include "engines/engine_components/eval_functions/f_cost_evaluator.h"
#include "engines/engine_components/eval_functions/weighted_f_cost_evaluator.h"
#include "engines/engine_components/node_containers/node_list.h"
#include "environments/sliding_tile_puzzle/sliding_tile_action.h"
#include "environments/sliding_tile_puzzle/sliding_tile_manhattan_heuristic.h"
#include "environments/sliding_tile_puzzle/sliding_tile_state.h"
#include "environments/sliding_tile_puzzle/sliding_tile_transitions.h"
#include "search_basics/node_container.h"
#include "utils/string_utils.h"

#include <iostream>
#include <optional>

using std::cout;

int main() {
    // READ THROUGH BOTH THE COMMENTS AND CODE BELOW

    cout << "\n------ TUTORIAL OBJECTIVES ------\n";
    cout << "In this tutorial, you will walk through the definition of nodes, how they are stored and accessed.\n";
    cout << "and how evaluators (heuristic and evaluation functions) are used.\n";

    cout << "\n\n------ PART 1: Node Containers and Node IDs ------\n";
    // In HSEF, by node we mean a state and a path to that state (ie. a pointer to the parent node).
    // Instead of storing nodes individually, we use a ``NodeContainer" to store a collection of nodes.
    // NodeContainer is an abstract class defined in "core/search_basics/node_container.h". This allows
    // for algorithm specific NodeContainers to be defined.
    //
    // Importantly, nodes in a NodeContainer have an ID. The ID is just a unsigned integer that is unique
    // to the node and allows easy and efficient access to different nodes. When a node is created, its
    // ID is returned
    //
    // We will use a NodeList for the Sliding Tile Puzzle to show the functionality here
    // This is defined in "engines/engine_components/node_containers/node_list.h"
    //
    // First, we will create 4 states. Notice that state_2 and state_3 can be reached from state_1 in a single move.
    // state_4 can then be reached from state_3 in one move
    SlidingTileState state_1({1, 2, 3, 4, 5, 8, 7, 0, 6}, 3, 3);
    SlidingTileState state_2({1, 2, 3, 4, 5, 8, 7, 6, 0}, 3, 3);
    SlidingTileState state_3({1, 2, 3, 4, 5, 8, 0, 7, 6}, 3, 3);
    SlidingTileState state_4({1, 2, 3, 0, 5, 8, 4, 7, 6}, 3, 3);

    cout << "tile_state_1: " << state_1 << "\n";
    cout << "tile_state_2: " << state_2 << "\n";
    cout << "tile_state_2: " << state_3 << "\n";
    cout << "tile_state_2: " << state_4 << "\n\n";

    // We will now create a NodeList
    NodeList<SlidingTileState, BlankSlide> nodes;

    // We will now add state_1
    // Here, state_1 has no parent
    NodeID node_1_id = nodes.addNode(state_1);
    cout << "The ID of state_1: " << node_1_id << "\n";

    // Now we will add state_2
    // Internally, the parent of state_2 is stored as the ID of state_1. Thus this is passed to this method
    // In addition, we pass the g-cost of the new state, the action that led to this state (ie. the action applied to
    // state_1 to yield state_2), and the cost of that action.
    NodeID node_2_id = nodes.addNode(state_2, node_1_id, 1.0, BlankSlide::right, 1.0);
    cout << "The ID of state_2: " << node_2_id << "\n";

    // We will now add the other states
    NodeID node_3_id = nodes.addNode(state_3, node_1_id, 1.0, BlankSlide::left, 1.0);
    cout << "The ID of state_3: " << node_3_id << "\n";

    NodeID node_4_id = nodes.addNode(state_4, node_3_id, 2.0, BlankSlide::up, 1.0);
    cout << "The ID of state_4: " << node_4_id << "\n\n";

    // You will notice that nodes all have unique IDs. We note that NodeContainers do not, by default, check that each
    // node corresponds to a unique state, meaning multiple nodes could be created for the same state. However, this
    // is generally discouraged.
    //
    // NodeContainers have a number of getters that now allow us to access different attributes of a node
    cout << "the state for node 4: " << nodes.getState(node_4_id) << "\n";
    cout << "the g-value of node 4: " << nodes.getGValue(node_4_id) << "\n";
    cout << "the id of the parent of node 4: " << nodes.getParentID(node_4_id) << "\n";  // If a node has no parent, its parent ID is its own ID

    // Since initial nodes don't have last actions, this returns an std::nullopt for initial nodes
    std::optional<BlankSlide> last_action = nodes.getLastAction(node_4_id);
    cout << "node 4 has a parent: " << boolToString(last_action.has_value()) << "\n";
    if (last_action.has_value()) {
        cout << "the action that led to node 4 is: " << boolToString(last_action.has_value()) << "\n";
    }
    cout << "the cost of the action that led to node 4 is: " << nodes.getLastActionCost(node_4_id) << "\n";

    // There are also setters for g-value, parent ID, last action, and last action cost to allow the path to a node
    // to be updated during a search

    //
    //
    cout << "\n\n------ PART 2: Node Evaluator Basics ------\n";
    // Node evaluators provide a way to evaluate a node, which can be used by a search algorithm as it sees fit.
    // This will include heuristic functions (like Manhattan distance) and evaluation functions (like f-cost).
    // The abstract class defining an evaluator can be found in "core/search_basics/node_evaluator.h"
    //
    // Let's begin by the creating an object to compute the Manhattan Distance of a state
    // The function needs to know the desired goal as well as the action-cost model
    SlidingTileState goal(3, 3);  // The default state is the standard goal [0 1 2, 3 4 5, 6, 7, 8]
    SlidingTileManhattanHeuristic manhattan(goal, SlidingTileCostType::unit);

    // Because evaluators work on nodes and not directly on states (though they may ignore non-state info,
    // as is done by the Manhattan Distance heuristic), an evaluator needs to be associated with a NodeContainer.
    // This allows the evaluator to access all needed information when calculating a heuristic value
    // We do this as follows
    manhattan.setNodeContainer(nodes);

    // The evaluator is now ready to compute values. This is a two-step process. The first is that the evaluator
    // prepares for an evaluation. Next the evaluator actually performs the evaluation
    // The importance of the preparation step will become clear in Part 3 of this tutorial.
    // Notice that the call to evaluate takes in the node ID of the node to evaluate.
    manhattan.prepareToEvaluate();
    manhattan.evaluate(node_1_id);

    // We can now access the evaluation and relevant information
    cout << "Evaluation for node " << node_1_id << "\n";
    cout << "Has an evaluation been completed: " << boolToString(manhattan.isEvalComputed()) << "\n";
    cout << "The ID of the last node evaluated: " << manhattan.getIDofLastEvaluatedNode() << "\n";
    cout << "Is the last node evaluated a dead-end: " << boolToString(manhattan.isLastNodeADeadEnd()) << "\n";
    cout << "Last computed evaluation: " << manhattan.getLastNodeEval() << "\n\n";

    // Let's evaluate the other nodes as well
    manhattan.prepareToEvaluate();
    manhattan.evaluate(node_2_id);

    cout << "Evaluation for node " << node_2_id << "\n";
    cout << "Has an evaluation been completed: " << boolToString(manhattan.isEvalComputed()) << "\n";
    cout << "The ID of the last node evaluated: " << manhattan.getIDofLastEvaluatedNode() << "\n";
    cout << "Is the last node evaluated a dead-end: " << boolToString(manhattan.isLastNodeADeadEnd()) << "\n";
    cout << "Last computed evaluation: " << manhattan.getLastNodeEval() << "\n\n";

    manhattan.prepareToEvaluate();
    manhattan.evaluate(node_3_id);

    cout << "Evaluation for node " << node_3_id << "\n";
    cout << "Has an evaluation been completed: " << boolToString(manhattan.isEvalComputed()) << "\n";
    cout << "The ID of the last node evaluated: " << manhattan.getIDofLastEvaluatedNode() << "\n";
    cout << "Is the last node evaluated a dead-end: " << boolToString(manhattan.isLastNodeADeadEnd()) << "\n";
    cout << "Last computed evaluation: " << manhattan.getLastNodeEval() << "\n\n";

    manhattan.prepareToEvaluate();
    manhattan.evaluate(node_3_id);

    cout << "Evaluation for node " << node_4_id << "\n";
    cout << "Has an evaluation been completed: " << boolToString(manhattan.isEvalComputed()) << "\n";
    cout << "The ID of the last node evaluated: " << manhattan.getIDofLastEvaluatedNode() << "\n";
    cout << "Is the last node evaluated a dead-end: " << boolToString(manhattan.isLastNodeADeadEnd()) << "\n";
    cout << "Last computed evaluation: " << manhattan.getLastNodeEval() << "\n\n";

    // Importantly, evaluators are responsible for caching an evaluation
    // As such, we can access previously evaluations as well
    // This is useful for ordering node IDs in an open list, for example
    cout << "The evaluation of node 3: " << manhattan.getCachedEval(node_3_id) << "\n";
    cout << "Is node 3 a dead end: " << boolToString(manhattan.getCachedIsDeadEnd(node_3_id)) << "\n";

    //
    //
    cout << "\n\n------ PART 2: Composite Evaluators ------\n";
    // Evaluation functions like f-cost are composite evaluation functions, since they require the output of
    // another evaluation function (namely, the heuristic function)
    // HSEF is set up allow for such composite functions in a way that avoids duplicate computation.
    // This is the purpose of the "prepareToEvaluate" call
    //
    // To show this, lets consider two evaluation functions: an f-cost and a weighted f-cost evaluator (with weight 2)
    // Notice that they both use the same Manhattan distance evaluator for their base heuristic
    FCostEvaluator<SlidingTileState, BlankSlide> f_cost(manhattan);
    f_cost.setNodeContainer(nodes);

    WeightedFCostEvaluator<SlidingTileState, BlankSlide> weight2_f_cost(manhattan, 2.0);
    weight2_f_cost.setNodeContainer(nodes);

    // Let's reset the evaluator so that all are empty
    manhattan.reset();

    // Now suppose we want to evaluate both the f_cost and weight2_f_cost evaluations for node 1
    // It would be inefficient to need to calculate the Manhattan distance twice. Instead, we want to calculate
    // it once and reuse it for the bother evaluation functions. This is enabled by prepareToEvaluate.
    // In particular, by calling it for the two evaluation functions, this will call the prepare for the corresponding
    // heuristic function
    //
    // More specifically, prepareToEvaluate switches an evaluator from "have computed value" to "ready to compute value" mode
    // This enables the reuse of value.
    f_cost.prepareToEvaluate();
    weight2_f_cost.prepareToEvaluate();

    // We can now evaluate f_cost
    // This will also have manhattan evaluate since this is needed for f_cost
    f_cost.evaluate(node_1_id);

    cout << "The manhattan distance for node 1: " << manhattan.getLastNodeEval() << "\n";
    cout << "The f-cost of node 1: " << manhattan.getLastNodeEval() << "\n";

    // We can now also call weight2_f_cost to evaluate. Importantly, it will just reuse the previously computed value
    // This is because internally, manhattan will be in "have computed value" mode
    weight2_f_cost.evaluate(node_1_id);
    cout << "The weighted f-cost of node 1: " << weight2_f_cost.getLastNodeEval() << "\n";

    //
    //
    cout << "\n------ PART 3: Re-Evaluation ------\n";
    // If a new path to a node is found, a node may need to be re-evaluated. For example, a new best path
    // will decrease its f-cost. However, we do not want to re-evaluate the underlying heuristic in the
    // case of Manhattan distance. We just want to reuse the pre-computed value instead.
    // This is accomplished by reEvaluate
    //
    // Let's evaluate node 4
    f_cost.prepareToEvaluate();
    f_cost.evaluate(node_4_id);

    cout << "The g-value of node 4: " << nodes.getGValue(node_4_id) << "\n";
    cout << "The h-value of node 4: " << manhattan.getCachedEval(node_4_id) << "\n";
    cout << "The f-value of node 4: " << f_cost.getCachedEval(node_4_id) << "\n\n";

    // Let us now update the g-cost of node 4.
    nodes.setGValue(node_4_id, 1);

    // In practice, the parent and last action would likely also be updated here, but that is not
    // important for this example
    //
    // Now we will call prepare and then reEvaluate
    f_cost.prepareToEvaluate();
    f_cost.reEvaluate(node_4_id);

    cout << "The g-value of node 4: " << nodes.getGValue(node_4_id) << "\n";
    cout << "The h-value of node 4: " << manhattan.getCachedEval(node_4_id) << "\n";
    cout << "The f-value of node 4: " << f_cost.getCachedEval(node_4_id) << "\n";

    // Importantly, the manhattan distance did not need to be re-evaluated here. It is set up to just re-use
    // the previously stored value.
    //
    // Note, "evaluate(node_id)" will just ignore anything cached for a node if it is called, while
    // reEvaluate will reuse what it can

    //
    //
    cout << "\n\n------ SUMMARY ------\n";
    cout << " - Nodes are stored in NodeContainers, which maintain state and path information.\n";
    cout << " - NodeEvaluators provide evaluations of different nodes and so are associated with containers.\n";
    cout << " - Evaluating nodes is a two step process which enables reuse of computed values for composite evaluators.\n";
    cout << " - ReEvaluating nodes allows us to reuse parts of the computation that are not path-specific.\n";

    return 0;
}