#include "grid_pathfinding_utils.h"
#include "grid_location.h"
#include "grid_names.h"
#include "grid_pathfinding_transitions.h"
#include "utils/string_utils.h"

#include <iostream>
#include <regex>
#include <string>

using std::cerr;
using std::istream;
using std::stoi;
using std::string;
using std::vector;

std::ostream& operator<<(std::ostream& out, const GridPathfindingCostType& cost_type) {
    switch (cost_type) {
        case GridPathfindingCostType::standard:
            out << gridNames::COST_STANDARD;
            break;
        case GridPathfindingCostType::life:
            out << gridNames::COST_LIFE;
            break;
    }
    return out;
}

bool readInGridPathfindingProblems(std::istream& file_stream, vector<GridLocation>& starts,
          vector<GridLocation>& goals) {
    string new_line;
    vector<string> tokens;

    std::regex valid_integer("([1-9][0-9]*)|0");
    vector<GridLocation> possible_starts, possible_goals;

    while (getline(file_stream, new_line)) {
        tokens = split(new_line, ' ');

        if (tokens.size() != 4) {
            cerr << "math_pathfinding_utils::readInGridPathfindingProblems - Invalid problem file format. All lines must be of length 4.\n";
            return false;
        }

        string start_x = tokens[0];
        string start_y = tokens[1];

        string goal_x = tokens[2];
        string goal_y = tokens[3];

        if (!(regex_match(start_x, valid_integer) && regex_match(start_y, valid_integer) &&
                      regex_match(goal_x, valid_integer) && regex_match(goal_y, valid_integer))) {
            cerr << "math_pathfinding_utils::readInGridPathfindingProblems - Invalid problem file format."
                 << "Input must only contain non-negative integers.\n";
            return false;
        }

        possible_starts.emplace_back(stoi(start_x), stoi(start_y));
        possible_goals.emplace_back(stoi(goal_x), stoi(goal_y));
    }

    for (unsigned i = 0; i < possible_starts.size(); i++) {
        starts.push_back(possible_starts[i]);
        goals.push_back(possible_goals[i]);
    }

    return true;
}

std::ostream& operator<<(std::ostream& out, const GridConnectionType& connection_type) {
    switch (connection_type) {
        case GridConnectionType::four:
            out << gridNames::FOUR_CONNECTED;
            break;
        case GridConnectionType::eight:
            out << gridNames::EIGHT_CONNECTED;
            break;
    }
    return out;
}
