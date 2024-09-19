#include "string_utils.h"
#include "search_basics/search_engine.h"

#include <cassert>
#include <cmath>
#include <cstddef>
#include <ostream>
#include <sstream>
#include <string>

using std::string;
using std::stringstream;
using std::vector;

string boolToString(bool value) {
    if (value) {
        return "true";
    }
    return "false";
}

vector<string> split(const string& str, char delim) {
    stringstream str_ss(str);
    string item;
    vector<string> tokens;

    while (getline(str_ss, item, delim)) {
        tokens.push_back(item);
    }
    return tokens;
}

string roundAndToString(double value, unsigned precision) {
    if (precision > 6) {
        return "";
    }

    // Check if the value is an integer and precision is 0
    if (std::floor(value) == value) {
        if (precision == 0) {
            return std::to_string(static_cast<int>(value));
        } else {
            return std::to_string(static_cast<int>(value)) + ".0";
        }
    }

    double factor = pow(10, precision);
    string result = std::to_string((round(value * factor) / factor));

    size_t num_included_chars = result.find('.') + 1 + precision;
    if (precision == 0) {
        num_included_chars--;
    }

    // Check for trailing zeros after the decimal point
    size_t decimal_pos = result.find('.');
    if (decimal_pos != string::npos) {
        size_t last_zero = result.find_last_not_of('0');
        if (last_zero != string::npos && last_zero > decimal_pos) {
            result = result.substr(0, last_zero + 1);
        }
    }

    // If there are no digits after the decimal point, append "0"
    if (result.back() == '.') {
        result += "0";
    }

    return result.substr(0, num_included_chars);
}

int findCharOccurance(const string& str, char character, unsigned occurance) {
    assert(occurance >= 1);
    size_t pos = 0;
    unsigned num_occurances = 0;

    while (pos < str.size() && num_occurances != occurance) {
        if (str[pos] == character) {
            num_occurances++;
        }
        if (num_occurances == occurance) {
            return static_cast<int>(pos);
        }
        pos++;
    }
    return -1;
}

std::ostream& operator<<(std::ostream& out, const EngineStatus& status) {
    if (status == EngineStatus::not_ready) {
        out << "not_ready";
    } else if (status == EngineStatus::ready) {
        out << "ready";
    } else if (status == EngineStatus::active) {
        out << "active";
    } else if (status == EngineStatus::search_completed) {
        out << "search_completed";
    } else if (status == EngineStatus::resource_limit_hit) {
        out << "resource_limit_hit";
    }
    return out;
}