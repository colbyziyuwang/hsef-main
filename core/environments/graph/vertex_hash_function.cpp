#include "vertex_hash_function.h"
#include "graph_state.h"

#include <cstdint>

uint32_t VertexHashFunction::getHashValue(const GraphState& state) const {
    return state.m_vertex_id;
}
