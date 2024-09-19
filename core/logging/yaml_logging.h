#ifndef YAML_LOGGING_H_
#define YAML_LOGGING_H_

#include "logging/search_component_settings.h"

#include <sstream>
#include <string>
#include <vector>

/**
 * Returns a string in YAML format containing the data from the SearchComponentSettings and the sub-components
 */
std::string getYAMLString(const SearchComponentSettings& settings, std::string prefix = "");

/**
*  Namespace containing utility functions for creating strings for yaml files
*/
namespace yamlHelperFunctions {
    inline const std::string LIST_PREFIX = "- ";  ///< The string for the beginning of a list entry in YAML syntax
    inline const std::string DICTIONARY_DELIM = ": ";  ///< The string that delimits the key and value of a dictionary in YAML syntax

    /**
    * Adds indentation to the provided string to allow representation of member entries
    *
    * @param prefix The indentation string that is used before data is added
    */
    inline void startMembers(std::string& prefix) {
        prefix.append("\t");
    }

    /**
    * Removes indentation from the provided string
    *
    * @param prefix The indentation string that is used before data is added
    */
    inline void endMembers(std::string& prefix) {
        if (!prefix.empty()) {
            prefix.pop_back();
        }
    }

    /**
    * Takes 1 or 2 strings and returns the YMAL dictionary represenation
    *
    * @param key The key value of the dictionary
    * @param value The value associated to the key
    */
    inline std::string dictionaryStr(const std::string& key, const std::string& value = "") {
        return key + DICTIONARY_DELIM + value;
    }

    /**
    * Takes a string a returns the list entry representation of it
    *
    * @param item The string to be added as a list item
    */
    inline std::string listEntry(const std::string& item) {
        return LIST_PREFIX + item;
    }

    /**
    * Converts a vector to a string that follows ymal formatting
    */
    template<class Element_t>
    std::string vectorToYAMLString(const std::vector<Element_t>& vec) {
        std::stringstream str_ss;
        str_ss << "[";
        for (unsigned i = 0; i < vec.size(); i++) {
            str_ss << "'" << vec[i] << "'";
            if (i != vec.size() - 1) {
                str_ss << ",";
            }
        }
        str_ss << "]";
        return str_ss.str();
    }

}  // namespace yamlHelperFunctions
#endif  //YAML_LOGGING_H_
