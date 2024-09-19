#ifndef TEST_HELPERS_H_
#define TEST_HELPERS_H_

#include <iostream>

#include "building_tools/evaluators/cost_and_distance_to_go_evaluator.h"
#include "engines/engine_components/node_containers/node_list.h"
#include "search_basics/node_container.h"
#include "search_basics/node_evaluator.h"
#include "utils/floating_point_utils.h"

/**
 * Checks the node evaluation is correct.
 *
 * @param evaluator The evaluator to test.
 * @param node_id The ID of the node to test.
 * @param expected_eval The expected evaluation.
 * @param expected_is_dead_end The expected value of if the node is a dead end
 * @param re_evaluate Whether to evaluate the node or re-evaluate the node
 */
template<class State_t, class Action_t>
bool checkNodeEvaluation(NodeEvaluator<State_t, Action_t>& evaluator, NodeID node_id,
          double expected_eval, bool expected_is_dead_end, bool re_evaluate = false);

/**
 * Checks that the state evaluation computes the expected values.
 *
 * @param evaluator The evaluator
 * @param state The state to test one
 * @param expected_eval The expected evaluation
 * @param expected_is_dead_end The expected output of is it a dead end.
 */
template<class State_t, class Action_t>
bool checkStateEvaluation(NodeEvaluator<State_t, Action_t>& evaluator, const State_t& state,
          double expected_eval, bool expected_is_dead_end);

/**
 * Checks that the state evaluation and distance-to-go evaluation computes as expected.
 * @param evaluator The evaluator
 * @param state The state to test one
 * @param expected_eval The expected evaluation
 * @param expected_is_dead_end The expected output of is it a dead end.
 * @param expected_distance The expected distance-to-go-estimate.
 */
template<class State_t, class Action_t>
bool checkDistanceToGoStateEvaluation(CostAndDistanceToGoEvaluator<State_t, Action_t>& evaluator, const State_t& state,
          double expected_eval, bool expected_is_dead_end, double expected_distance);

template<class State_t, class Action_t>
bool checkStateEvaluation(NodeEvaluator<State_t, Action_t>& evaluator, const State_t& state,
          double expected_eval, bool expected_is_dead_end) {
    NodeList<State_t, Action_t> nodes;
    evaluator.setNodeContainer(nodes);
    NodeID node_id = nodes.addNode(state);

    return checkNodeEvaluation(evaluator, node_id, expected_eval, expected_is_dead_end, false);
}

template<class State_t, class Action_t>
bool checkDistanceToGoStateEvaluation(CostAndDistanceToGoEvaluator<State_t, Action_t>& evaluator, const State_t& state,
          double expected_eval, bool expected_is_dead_end, double expected_distance) {
    bool state_check = checkStateEvaluation(evaluator, state, expected_eval, expected_is_dead_end);

    if (!fpEqual(evaluator.getLastDistanceToGoEval(), expected_distance)) {
        std::cout << "---- TEST FAILS: Incorrect evaluation\n";
        std::cout << "        OUTPUT:   " << evaluator.getLastDistanceToGoEval() << "\n";
        std::cout << "        EXPECTED: " << expected_distance << "\n";
        return false;
    }

    return state_check;
}

template<class State_t, class Action_t>
bool checkNodeEvaluation(NodeEvaluator<State_t, Action_t>& evaluator, NodeID node_id,
          double expected_eval, bool expected_is_dead_end, bool re_evaluate) {

    evaluator.prepareToEvaluate();
    if (evaluator.isEvalComputed()) {
        std::cout << " ---- TEST FAILS: isEvalComputed() returns true after prepareToEvaluate is called\n";
        return false;
    }

    if (re_evaluate) {
        evaluator.reEvaluate(node_id);
    } else {
        evaluator.evaluate(node_id);
    }
    if (!evaluator.isEvalComputed()) {
        std::cout << "---- TEST FAILS: isEvalComputed() returns true after prepareToEvaluate is called\n";
        return false;
    }

    if (!fpEqual(evaluator.getLastNodeEval(), expected_eval)) {
        std::cout << "---- TEST FAILS: Incorrect evaluation\n";
        std::cout << "        OUTPUT:   " << evaluator.getLastNodeEval() << "\n";
        std::cout << "        EXPECTED: " << expected_eval << "\n";
        return false;
    }

    if (evaluator.isLastNodeADeadEnd() != expected_is_dead_end) {
        std::cout << "---- TEST FAILS: Incorrect isDeadEnd\n";
        std::cout << "        OUTPUT:   " << evaluator.isLastNodeADeadEnd() << "\n";
        std::cout << "        EXPECTED: " << expected_is_dead_end << "\n";
        return false;
    }
    return true;
}

#endif  //TEST_HELPERS_H_
