#ifndef BEST_FIRST_SEARCH_PARAMS_H_
#define BEST_FIRST_SEARCH_PARAMS_H_

#include "logging/logging_terms.h"


/**
 * The parameters for a best first search
 */
struct BestFirstSearchParams {

    /**
     * Returns a map containing the log paramater that use in Best First Search
     * @return A map to stand for the Log of paramas
     */
    StringMap getParameterLog() const;

    bool m_use_reopened = true;  ///< Whether we are reopening closed nodes
    bool m_store_expansion_order = false;  ///< Whether we want to store the order of node expansions
};
#endif  //BEST_FIRST_SEARCH_PARAMS_H_