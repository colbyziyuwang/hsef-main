/**
 * Utilities for string manipulation.
 *
 * @file string_utils.h
 */

#ifndef STRING_UTILS_H_
#define STRING_UTILS_H_

#include "search_basics/search_engine.h"
#include <cstdlib>
#include <ostream>
#include <sstream>
#include <string>
#include <vector>

/**
 * Converts the given boolean to a string.
 *
 * @param value The boolean to convert.
 * @return The string representation of the boolean.
 */
std::string boolToString(bool value);

/**
 * Splits the given string according into different tokens according to the given delimiter,
 * and returns them as a vector.
 *
 * @param str The string to split.
 * @param delim The delimiter.
 * @return A vector of tokens
 */
std::vector<std::string> split(const std::string& str, char delim);

/**
 * Returns a string representation of the given vector. Output looks like "[e0 e1 ... ek]" where ei is the string
 * representation of each element.
 *
 * Assumes that the "<<" operator can be applied to the vector elements.
 *
 * @param vec A vector of elements.
 * @return A string representation of the vector.
 */
template<class Element_t>
std::string vectorToString(const std::vector<Element_t>& vec);

/**
 * Rounds a floating point value to the specified decimal place and returns the string 
 * 
 * The precision must be between 0 and 6, inclusive or an empty string will be returned
 * 
 * @param value The double value to be converted and rounded
 * @param precision The amount of decimal places to round to
 */
std::string roundAndToString(double value, unsigned precision);

/**
* Finds the index of the nth occurance of the specified character in the string
* returns -1 if the nth occurance does not exist
* 
* @param str The string to search in
* @param character The character being looked for
* @param occurance The nth occurance of the character being searched for
*/
int findCharOccurance(const std::string& str, char character, unsigned occurance);

/**
 * Takes in a streamable element and generates the String for it.
 *
 * @tparam Element_t The type of the element
 * @param element The element itself
 * @return A string representation of the given element.
 */
template<class Element_t>
std::string streamableToString(const Element_t& element);

/**
 * Defines an output representation of an engine status.
 *
 * @param out The output stream.
 * @param status The status to output.
 * @return A string representation of the status.
 */
std::ostream& operator<<(std::ostream& out, const EngineStatus& status);

// Templated function implementations
template<class Element_t>
std::string vectorToString(const std::vector<Element_t>& vec) {
    std::stringstream str_ss;
    str_ss << "[";
    for (unsigned i = 0; i < vec.size(); i++) {
        str_ss << vec[i];
        if (i != vec.size() - 1) {
            str_ss << " ";
        }
    }
    str_ss << "]";
    return str_ss.str();
}

template<class Element_t>
std::string streamableToString(const Element_t& element) {
    std::stringstream str_ss;
    str_ss << element;
    return str_ss.str();
}

#endif /* STRING_UTILS_H_ */
