#ifndef SEARCH_COMPONENT_SETTINGS_H_
#define SEARCH_COMPONENT_SETTINGS_H_

#include <map>
#include <string>

#include "logging_terms.h"

struct SearchComponentSettings;
using SearchSettingsMap = std::map<std::string, SearchComponentSettings>;  ///< A map of the search settings

/**
 * Stores the settings of some component and its sub-components.
 */
struct SearchComponentSettings {
    std::string m_name;  ///< The name of the component (i.e., ManhattanDistanceHeuristic)
    StringMap m_main_settings;  ///< The settings for this component (ie. the weight for this weighted f-cost evaluator)
    SearchSettingsMap m_sub_component_settings;  ///< The settings for sub-components (ie. can index the settings for the evaluator of BestFirstSearch using "evaluator")
};

#endif  // SEARCH_COMPONENT_SETTINGS_H_
