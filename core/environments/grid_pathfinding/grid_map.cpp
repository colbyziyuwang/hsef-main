#include "grid_map.h"
#include "utils/io_utils.h"
#include "utils/string_utils.h"

#include <cassert>
#include <iostream>
#include <optional>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

using std::cerr;
using std::string;
using std::vector;

GridMap::GridMap(int width, int height)
          : m_map_width(width), m_map_height(height),
            m_grid(width, std::vector<GridLocationType>(height, GridLocationType::passable)) {
}

GridMap::GridMap(std::istream& grid_map_stream) {
    bool load_result = loadMap(grid_map_stream);
    assert(load_result);
}

GridMap::GridMap(const std::string& file_name) {
    std::stringstream temp_file_ss = loadFileIntoStringSteam(file_name);
    bool load_result = loadMap(temp_file_ss);
    assert(load_result);
}

GridLocationType GridMap::getLocationType(int x_coord, int y_coord) const {
    if (isInMap(x_coord, y_coord)) {
        return m_grid[x_coord][y_coord];
    }

    return GridLocationType::outside_grid;
}

int GridMap::getWidth() const {
    return m_map_width;
}

int GridMap::getHeight() const {
    return m_map_height;
}

bool GridMap::loadMap(std::istream& grid_map_stream) {
    m_map_width = -1;
    m_map_height = -1;

    // Extracts header and resizes grid if can
    auto [read_succeeded, line_count] = extractHeaderFromStream(grid_map_stream);

    // If succeeded reads in the locations
    if (read_succeeded) {
        m_grid.resize(m_map_width, vector<GridLocationType>(m_map_height, GridLocationType::passable));

        string new_line;
        vector<string> tokens;
        int current_row = 0;

        while (read_succeeded && getline(grid_map_stream, new_line)) {
            new_line.erase(new_line.find_last_not_of("\t\n\v\f\r ") + 1);  // gets rid of whitespace at end

            if (current_row >= m_map_height) {
                cerr << "Given map has more rows than the given specified height.\n";
                cerr << "Map reading failed.\n";
                read_succeeded = false;
            }
            line_count++;

            if (new_line.size() != static_cast<unsigned>(m_map_width)) {
                cerr << "Line " << line_count << " of map file has incorrect length for the specified width.\n";
                cerr << "Map reading failed.\n";
                read_succeeded = false;
            }

            for (unsigned i = 0; i < new_line.size(); i++) {
                std::optional<GridLocationType> location_type = convertCharToLocationType(new_line.at(i));
                if (location_type.has_value()) {
                    m_grid[i][current_row] = location_type.value();
                } else {
                    cerr << "Invalid map location symbol " << new_line.at(i) << " on line " << line_count << " of map file.\n";
                    cerr << "Map reading failed.\n";
                    read_succeeded = false;
                    break;
                }
            }
            current_row++;
        }

        if (current_row < m_map_height) {
            cerr << "Given map has fewer rows than the given specified height.\n";
            cerr << "Map reading failed.\n";
            read_succeeded = false;
        }
    }

    if (!read_succeeded) {
        clearMap();
    }
    return read_succeeded;
}

void GridMap::clearMap() {
    m_grid.clear();
    m_map_width = 0;
    m_map_height = 0;
}

std::pair<bool, int> GridMap::extractHeaderFromStream(std::istream& grid_map_stream) {
    string new_line;
    vector<string> tokens;

    int line_count = 0;

    bool extract_succeed = true;
    while (extract_succeed && getline(grid_map_stream, new_line)) {
        new_line.erase(new_line.find_last_not_of("\t\n\v\f\r ") + 1);  // gets rid of whitespace at end
        line_count++;
        if (new_line == "map") {
            break;
        } else if (new_line != "type octile") {
            tokens = split(new_line, ' ');

            if (!tokens.empty() && (tokens[0] == "width" || tokens[0] == "height")) {
                extract_succeed = extractDimensionFromHeaderLine(tokens);
            } else {
                extract_succeed = false;
            }
        }
    }
    if (m_map_height <= 0 || m_map_width <= 0) {
        cerr << "Map width or height not specified in map file.\nMap reading failed.\n";
        extract_succeed = false;
    }

    if (!extract_succeed) {
        clearMap();
    }

    return {extract_succeed, line_count};
}

bool GridMap::extractDimensionFromHeaderLine(const vector<string>& line_tokens) {
    if (line_tokens.size() > 2) {
        cerr << "Improper formatting of " << line_tokens[0] << " in file.\nGrid map reading failed\n";
        return false;
    }

    int dimension_value = stoi(line_tokens[1]);
    if (dimension_value <= 0) {
        cerr << "Invalid " << line_tokens[0] << "entered in file.\nGrid map reading failed.\n";
        return false;
    }

    if ((line_tokens[0] == "width" && m_map_width > 0) || (line_tokens[0] == "height" && m_map_height > 0)) {
        cerr << "Map grid " << line_tokens[0] << " entered multiple times in file.\nGrid map reading failed.\n";
        return false;
    } else if (line_tokens[0] == "width") {
        m_map_width = dimension_value;
    } else {
        m_map_height = dimension_value;
    }
    return true;
}

std::optional<GridLocationType> GridMap::convertCharToLocationType(char to_convert) {
    switch (to_convert) {
        case '.':
        case 'G':
            return GridLocationType::passable;
        case '@':
        case 'O':
            return GridLocationType::obstacle;
        case 'T':
            return GridLocationType::tree;
        case 'S':
            return GridLocationType::swamp;
        case 'W':
            return GridLocationType::water;
        default:
            break;
    }
    return std::nullopt;
}

bool GridMap::isInMap(int x_coord, int y_coord) const {
    return x_coord >= 0 && x_coord < m_map_width && y_coord >= 0 && y_coord < m_map_height;
}

bool GridMap::canOccupyLocation(int x_coord, int y_coord) const {
    return isInMap(x_coord, y_coord) && (m_grid[x_coord][y_coord] == GridLocationType::passable ||
                                                  m_grid[x_coord][y_coord] == GridLocationType::swamp ||
                                                  m_grid[x_coord][y_coord] == GridLocationType::water);
}

bool GridMap::isTraversable(int x_1, int y_1, int x_2, int y_2) const {
    if (m_grid[x_1][y_1] == GridLocationType::passable || m_grid[x_1][y_1] == GridLocationType::swamp) {
        return m_grid[x_2][y_2] == GridLocationType::passable ||
               m_grid[x_2][y_2] == GridLocationType::swamp;
    }

    return m_grid[x_2][y_2] == GridLocationType::water;
}

bool GridMap::canMove(int x_coord, int y_coord, int delta_x, int delta_y) const {
    assert(canOccupyLocation(x_coord, y_coord));

    return isInMap(x_coord + delta_x, y_coord + delta_y) &&
           isTraversable(x_coord, y_coord, x_coord + delta_x, y_coord + delta_y);
}

bool GridMap::canMoveNorth(int x_coord, int y_coord) const {
    return canMove(x_coord, y_coord, 0, -1);
}

bool GridMap::canMoveEast(int x_coord, int y_coord) const {
    return canMove(x_coord, y_coord, 1, 0);
}

bool GridMap::canMoveSouth(int x_coord, int y_coord) const {
    return canMove(x_coord, y_coord, 0, 1);
}

bool GridMap::canMoveWest(int x_coord, int y_coord) const {
    return canMove(x_coord, y_coord, -1, 0);
}

bool GridMap::canMoveNorthEast(int x_coord, int y_coord, bool check_four_way) const {
    return canMove(x_coord, y_coord, 1, -1) &&
           (!check_four_way || (canMoveNorth(x_coord, y_coord) && canMoveEast(x_coord, y_coord)));
}

bool GridMap::canMoveSouthEast(int x_coord, int y_coord, bool check_four_way) const {
    return canMove(x_coord, y_coord, 1, 1) &&
           (!check_four_way || (canMoveSouth(x_coord, y_coord) && canMoveEast(x_coord, y_coord)));
}

bool GridMap::canMoveSouthWest(int x_coord, int y_coord, bool check_four_way) const {
    return canMove(x_coord, y_coord, -1, 1) &&
           (!check_four_way || (canMoveSouth(x_coord, y_coord) && canMoveWest(x_coord, y_coord)));
}

bool GridMap::canMoveNorthWest(int x_coord, int y_coord, bool check_four_way) const {
    return canMove(x_coord, y_coord, -1, -1) &&
           (!check_four_way || (canMoveNorth(x_coord, y_coord) && canMoveWest(x_coord, y_coord)));
}
