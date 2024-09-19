#include "best_first_search_params.h"
#include "utils/string_utils.h"

StringMap BestFirstSearchParams::getParameterLog() const {
    StringMap params;

    params["use_reopened"] = boolToString(m_use_reopened);
    params["store_expansion_order"] = boolToString(m_store_expansion_order);
    return params;
}