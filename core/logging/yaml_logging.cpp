#include "logging/yaml_logging.h"
#include "utils/string_utils.h"

#include <algorithm>
#include <iostream>
#include <map>
#include <sstream>
#include <string>

using namespace yamlHelperFunctions;

std::string getYAMLString(const SearchComponentSettings& state, std::string prefix) {
    std::stringstream output_string("");
    char line_end = '\n';
    output_string << prefix << dictionaryStr("name", state.m_name) << line_end;
    if (!state.m_main_settings.empty()) {
        output_string << prefix << dictionaryStr("settings") << line_end;
        startMembers(prefix);
        std::map<std::string, std::string> settings(state.m_main_settings.begin(), state.m_main_settings.end());
        for (auto it = settings.rbegin(); it != settings.rend(); it++) {
            output_string << prefix << listEntry(dictionaryStr(it->first, it->second)) << line_end;
        }
        endMembers(prefix);
    }
    if (!state.m_sub_component_settings.empty()) {
        output_string << prefix << dictionaryStr("components") << line_end;
        startMembers(prefix);

        for (auto const& [role, component_settings] : state.m_sub_component_settings) {
            output_string << prefix << listEntry(dictionaryStr(role)) << line_end;
            startMembers(prefix);
            output_string << getYAMLString(component_settings, prefix);
            endMembers(prefix);
        }
        endMembers(prefix);
    }
    return output_string.str();
}