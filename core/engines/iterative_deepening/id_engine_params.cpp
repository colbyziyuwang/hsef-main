#include "id_engine_params.h"
#include "utils/string_utils.h"

StringMap IDEngineParams::getParameterLog() const {
    StringMap params;

    params["use_parent_pruning"] = boolToString(m_use_parent_pruning);
    params["use_random_op_ordering"] = boolToString(m_use_random_op_ordering);
    return params;
}
