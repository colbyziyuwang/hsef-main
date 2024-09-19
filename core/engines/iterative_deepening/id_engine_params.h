//
// Created by Rick Valenzano on 2021-08-20.
//

#ifndef ID_ENGINE_PARAMS_H_
#define ID_ENGINE_PARAMS_H_

#include "logging/logging_terms.h"

/**
 * The parameters for an iterative deepening engine
 */
struct IDEngineParams {
    /**
     * Returns a map containing the values of all the parameters
     */
    StringMap getParameterLog() const;

    bool m_use_parent_pruning = true;  ///< Whether or not to use parent pruning
    bool m_use_random_op_ordering = false;  ///< Whether or not to use random operator ordering
};
#endif  //ID_ENGINE_PARAMS_H_
