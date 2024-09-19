#ifndef GRAPH_TRANSITIONS_H_
#define GRAPH_TRANSITIONS_H_

#include "graph.h"
#include "graph_action.h"
#include "graph_state.h"
#include "logging/logging_terms.h"
#include "logging/search_component_settings.h"
#include "search_basics/transition_system.h"

#include <optional>
#include <string>
#include <vector>

/**
 * Defines the transitions for a graph environment.
 *
 * @class GraphTransitions
 */
class GraphTransitions : public TransitionSystem<GraphState, GraphAction> {
public:
    inline static const std::string CLASS_NAME = "GraphTransitions";  ///< The name of this class

    /**
     * Creates an object for GraphTransitions based on the given graph.
     *
     * @param graph The graph this transitions system will be based on
     */
    explicit GraphTransitions(const Graph& graph);

    /**
     * Default destructor.
     */
    ~GraphTransitions() override = default;

    /**
     * Gets the GraphTransitions state that corresponds to the vertex with the given name. This function is largely to
     * make running searches more convenient.
     *
     * Assumes that a vertex with that label is in the graph.
     *
     * @param vertex_label The label of the vertex to generate a state for
     * @return A state corresponding to the given vertex label
     */
    GraphState getVertexState(const std::string& vertex_label);

    /**
     * Gets the GraphTransitions action that corresponds to the edge between the vertices with the given vertex labels.
     * This function is largely to ake running searches more convenient.
     *
     * Assumes that an edge between the two vertices is in the graph.
     *
     * @param from_vertex_label The label of the from vertex to generate an action for
     * @param to_vertex_label The label of the to vertex to generate an action for
     * @return An action corresponding to the given edge
     */
    GraphAction getEdgeAction(const std::string& from_vertex_label, const std::string& to_vertex_label);

    // Overriden public TransitionSystem methods
    bool isApplicable(const GraphState& state, const GraphAction& action) const override;
    double getActionCost(const GraphState& state, const GraphAction& action) const override;
    void applyAction(GraphState& state, const GraphAction& action) const override;
    std::vector<GraphAction> getActions(const GraphState& state) const override;
    std::optional<GraphAction> getInverse(const GraphState& state, const GraphAction& action) const override;
    bool isValidState(const GraphState& state) const override;

    // Overriden public SettingsLogger methods
    std::string getName() const override { return CLASS_NAME; }

protected:
    // Overriden protected SettingsLogger methods
    StringMap getComponentSettings() const override { return {}; }
    SearchSettingsMap getSubComponentSettings() const override { return {}; }

private:
    const Graph* m_graph;  ///< The graph that the transitions correspond to
};

#endif /* GRAPH_TRANSITIONS_H_ */
