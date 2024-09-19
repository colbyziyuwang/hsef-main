#ifndef VERTEX_HASH_FUNCTION_H_
#define VERTEX_HASH_FUNCTION_H_

#include "building_tools/hashing/state_hash_function.h"
#include "graph_state.h"
#include "logging/logging_terms.h"
#include "logging/search_component_settings.h"

#include <cstdint>
#include <string>
#include <vector>

/**
 * A hash function for graph that is based on graph vertex state index.
 */
class VertexHashFunction : public StateHashFunction<GraphState, uint32_t> {

public:
    inline static const std::string CLASS_NAME = "PermutationHashFunction";  ///< The name of the class. Defines this component's name

    /**
     * Default destructor.
     */
    ~VertexHashFunction() override = default;

    uint32_t getHashValue(const GraphState& state) const override;
    bool isPerfectHashFunction() const override { return true; }

    // Overriden public SettingsLogger methods
    std::string getName() const override { return CLASS_NAME; }

protected:
    // Overriden protected SettingsLogger methods
    StringMap getComponentSettings() const override { return {}; };
    SearchSettingsMap getSubComponentSettings() const override { return {}; }
};
#endif /* VERTEX_HASH_FUNCTION_H_ */
