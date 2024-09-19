#ifndef GRIDMAP_H_
#define GRIDMAP_H_

#include <cstdint>
#include <iostream>
#include <optional>
#include <string>
#include <utility>
#include <vector>

/**
 * The different types of locations.
 */
enum class GridLocationType : std::uint8_t {
    passable,
    obstacle,
    tree,
    swamp,
    water,
    outside_grid,
};

/**
 * Stores a grid map.
 *
 * The origin (0,0) is set as the upper left corner in keeping with benchmark standard.
 * A location (x, y) means y rows from the top and x columns from the left.
 * Locations are either empty or have an obstacle. If the transition system is set as 4-connected,
 * then movement is only possible in the 4 cardinal directions (North, East, South, and West). If
 * it is not 4-connected, it is 8-connected, which means it is possible to move in the 4 cardinal
 * directions as well as Northeast, Southeast, Southwest, and Northwest.
 *
 * The file reading type is based on the MovingAI benchmark format.
 */
class GridMap {

public:
    /**
     * Creates an empty map (all locations are passable) with the given width and height.
     *
     * @param width The width of the map
     * @param height The height of the map
     */
    GridMap(int width, int height);

    /**
     * Generates a grid map from the given input stream containing a map file.
     *
     * @param grid_map_stream The grid map stream
     */
    explicit GridMap(std::istream& grid_map_stream);

    /**
     * Generates a grid map from the given input file.
     *
     * @param file_name The name of the file to input.
     */
    explicit GridMap(const std::string& file_name);

    /**
     * Gets the width of the map.
     *
     * @return The width of the map
     */
    int getWidth() const;

    /**
     * Gets the height of the map.
     *
     * @return The height of the map.
     */
    int getHeight() const;

    /**
     * Returns the type of location at the given coordinates.
     *
     * @param x_coord The x_coordinate
     * @param y_coord The y_coordinate
     * @return  The type of location
     */
    GridLocationType getLocationType(int x_coord, int y_coord) const;

    /**
     * Checks if the given location can be occupied. Requires that it be passable, swamp, or water.
     *
     * @param x_coord The x coordinate of the location to check
     * @param y_coord The y coordinate of the location to check
     * @return Whether the location can be occupied or not
     */
    bool canOccupyLocation(int x_coord, int y_coord) const;

    /**
     * Checks if a north move is possible from the given location.
     *
     * @param x_coord The x coordinate of the location to check
     * @param y_coord The y coordinate of the location to check
     * @return Whether a north move from the given location is possible
     */
    bool canMoveNorth(int x_coord, int y_coord) const;

    /**
     * Checks if an east move is possible from the given location.
     *
     * @param x_coord The x coordinate of the location to check
     * @param y_coord The y coordinate of the location to check
     * @return Whether an east move from the given location is possible
     */
    bool canMoveEast(int x_coord, int y_coord) const;

    /**
     * Checks if a south move is possible from the given location.
     *
     * @param x_coord The x coordinate of the location to check
     * @param y_coord The y coordinate of the location to check
     * @return Whether a south move from the given location is possible
     */
    bool canMoveSouth(int x_coord, int y_coord) const;

    /**
     * Checks if a west move is possible from the given location.
     *
     * @param x_coord The x coordinate of the location to check
     * @param y_coord The y coordinate of the location to check
     * @return Whether a west move from the given location is possible
     */
    bool canMoveWest(int x_coord, int y_coord) const;

    /**
     * Checks if a northeast move is possible from the given location.
     *
     * The four-way condition checks that the 4-way cardinal directions also don't have obstacles in the way.
     *
     * @param x_coord The x coordinate of the location to check
     * @param y_coord The y coordinate of the location to check
     * @param check_four_way Whether to check the 4-way cardinal directions for obstacles.
     * @return Whether a northeast move from the given location is possible
     */
    bool canMoveNorthEast(int x_coord, int y_coord, bool check_four_way = true) const;

    /**
     * Checks if a southeast move is possible from the given location.
     *
     * The four-way condition checks that the 4-way cardinal directions also don't have obstacles in the way.
     *
     * @param x_coord The x coordinate of the location to check
     * @param y_coord The y coordinate of the location to check
     * @param check_four_way Whether to check the 4-way cardinal directions for obstacles.
     * @return Whether a southeast move from the given location is possible
     */
    bool canMoveSouthEast(int x_coord, int y_coord, bool check_four_way = true) const;

    /**
     * Checks if a southwest move is possible from the given location.
     *
     * The four-way condition checks that the 4-way cardinal directions also don't have obstacles in the way.
     *
     * @param x_coord The x coordinate of the location to check
     * @param y_coord The y coordinate of the location to check
     * @param check_four_way Whether to check the 4-way cardinal directions for obstacles.
     * @return Whether a southwest move from the given location is possible
     */
    bool canMoveSouthWest(int x_coord, int y_coord, bool check_four_way = true) const;

    /**
     * Checks if a northwest move is possible from the given location.
     *
     * The four-way condition checks that the 4-way cardinal directions also don't have obstacles in the way.
     *
     * @param x_coord The x coordinate of the location to check
     * @param y_coord The y coordinate of the location to check
     * @param check_four_way Whether to check the 4-way cardinal directions for obstacles.
     * @return Whether a northwest move from the given location is possible
     */
    bool canMoveNorthWest(int x_coord, int y_coord, bool check_four_way = true) const;

private:
    /**
     * Checks if the given location is in the map or not.
     *
     * @param x_coord The x coordinate of the location to check
     * @param y_coord The y coordinate of the location to check
     * @return Whether the given location is in the map or not.
     */
    bool isInMap(int x_coord, int y_coord) const;

    /**
     * Checks if it is possible to traverse from the the first location to the second according to grid map rules.
     *
     * Assumes these are adjacent locations.
     *
     * @param x_1 The x coordinate of the first location
     * @param y_1 The y coordinate of the first location
     * @param x_2 The x coordinate of the second location
     * @param y_2 The y coordinate of the second location
     * @return If a traversal in the map is possible.
     */
    bool isTraversable(int x_1, int y_1, int x_2, int y_2) const;

    /**
     * Checks if we can move from the given location according to the given delta values. For the diagonal moves,
     * ignores obstacles along the regular 4-way cardinal directions.
     *
     * Assumes the delta values are all -1, 0, or 1
     *
     * @param x_coord The x coordinate of the location to check
     * @param y_coord The y coordinate of the location to check
     * @param delta_x The change in x to check
     * @param delta_y The change in y to check
     * @return If a move along the given delta is possible
     */
    bool canMove(int x_coord, int y_coord, int delta_x, int delta_y) const;

    /**
     * Clears the map because loading failed.
     */
    void clearMap();

    /**
     * Loads a map from the given input stream.
     *
     * @param grid_map_stream The stream to read the map from
     * @return Whether reading succeeded or not.
     */
    bool loadMap(std::istream& grid_map_stream);

    /**
     * Extracts the header from the given stream. Returns whether the extraction succeeded and the number of lines
     * during the extraction.
     *
     * The header should have the following format:
     *
     * height y
     * width x
     * map
     *
     * These define the height and width of the grid map. The keyword map defines when the map itself begins.
     *
     * @param grid_map_stream The stream with the grid map
     * @return Whether the extraction succeeded and the number of lines during the extraction.
     */
    std::pair<bool, int> extractHeaderFromStream(std::istream& grid_map_stream);

    /**
     * Extracts the dimension (height or width) from the given list of tokens and assigns the relevant dimension.
     * Returns whether the extraction succeeded or not.
     *
     * @param line_tokens The tokens for a line containing dimension information
     * @return Whether the extraction succeeded
     */
    bool extractDimensionFromHeaderLine(const std::vector<std::string>& line_tokens);

    /**
     * Converts the given character to a location type. Returns an null value if the character is invalid.
     *
     * @param to_convert The character to convert
     * @return The
     */
    static std::optional<GridLocationType> convertCharToLocationType(char to_convert);

    int m_map_width = -1;  ///< The map width.
    int m_map_height = -1;  ///< The map height.

    std::vector<std::vector<GridLocationType>> m_grid;  ///< Indexed by location, indicates if it is empty or an obstacle.
};


#endif  //GRIDMAP_H_
