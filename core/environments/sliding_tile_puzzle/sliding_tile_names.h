#ifndef SLIDING_TILE_NAMES_H_
#define SLIDING_TILE_NAMES_H_

#include <string>

namespace slidingTileNames {
    inline const std::string SETTING_COST_TYPE = "cost_type";  ///< The string for cost type setting
    inline const std::string COST_UNIT = "unit";  ///< The string for unit cost
    inline const std::string COST_HEAVY = "heavy";  ///< The string for heavy cost
    inline const std::string COST_INVERSE = "inverse";  ///< The string for inverse cost

    inline const std::string DIRECTION_UP = "up";  ///< The string for up direction
    inline const std::string DIRECTION_DOWN = "down";  ///< The string for down direction
    inline const std::string DIRECTION_RIGHT = "right";  ///< The string for right direction
    inline const std::string DIRECTION_LEFT = "left";  ///< The string for left direction

    inline const std::string SETTING_NUM_ROWS = "num_rows";  ///< The string for number of rows setting
    inline const std::string SETTING_NUM_COLS = "num_cols";  ///< The string for number of columns setting

    inline const std::string SETTING_GOAL_STATE = "goal_state";  ///< The goal state of the sliding tile puzzle
}  // namespace slidingTileNames

#endif  //SLIDING_TILE_NAMES_H_
