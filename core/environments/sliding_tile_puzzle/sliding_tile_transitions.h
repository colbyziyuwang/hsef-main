#ifndef SLIDING_TILE_TRANSITIONS_H_
#define SLIDING_TILE_TRANSITIONS_H_

#include "logging/logging_terms.h"
#include "logging/search_component_settings.h"
#include "search_basics/transition_system.h"
#include "sliding_tile_action.h"
#include "sliding_tile_state.h"

#include <cstdint>
#include <optional>
#include <string>
#include <vector>

/**
 * Defines the possible cost types for sliding tiles in the sliding tile puzzle domain.
 *
 * Below is the meaning of each of the values:
 *
 * unit - all actions have a cost of 1.\n
 * heavy - moving tile i has a cost of i.\n
 * inverse - moving tile i has a cost of 1/i.\n
 *
 * @class SlidingTileCostType
 */
enum class SlidingTileCostType : std::uint8_t {
    unit,
    heavy,
    inverse
};

/**
 * Defines the transition system for the sliding tile puzzle.
 *
 * @todo Add operator ordering options.
 * @todo Check what the default operator ordering should be.
 *
 * @class SlidingTileTransitions
 */
class SlidingTileTransitions : public TransitionSystem<SlidingTileState, BlankSlide> {
public:
    inline static const std::string CLASS_NAME = "SlidingTileTransitions";  ///< The name of the class. Defines this component's name

    /**
     * Builds a tile puzzle transition system with the given number of rows, columns, and cost type.
     *
     * @param rows The number of rows in the puzzle.
     * @param cols The number of columns in the puzzle.
     * @param cost_type The action cost type.
     */
    SlidingTileTransitions(int rows, int cols, SlidingTileCostType cost_type = SlidingTileCostType::unit);

    /**
     * Default destructor.
     */
    ~SlidingTileTransitions() override = default;

    // Overridden public TransitionSystem methods
    std::vector<BlankSlide> getActions(const SlidingTileState& state) const override;
    bool isApplicable(const SlidingTileState& state, const BlankSlide& action) const override;
    double getActionCost(const SlidingTileState& state, const BlankSlide& action) const override;
    void applyAction(SlidingTileState& state, const BlankSlide& action) const override;
    std::optional<BlankSlide> getInverse(const SlidingTileState& state, const BlankSlide& action) const override;
    bool isValidState(const SlidingTileState& state) const override;

    /**
     * Sets the puzzle dimensions.
     *
     * @param rows The number of rows in the puzzle
     * @param cols The numbef of columns in the puzzle
     */
    void setPuzzleDimensions(int rows, int cols);

    /**
     * Sets the tile costs to one of the standard types.
     *
     * @param cost_type The cost type for the transitions.
     */
    void setCostType(SlidingTileCostType cost_type);

    /**
     * Returns the cost of moving the specified tile.
     *
     * @param tile_num The tile to move.
     * @return The cost of moving the given tile.
     */
    double getTileMoveCost(Tile tile_num) const;

    // Overriden public SettingsLogger methods
    std::string getName() const override { return CLASS_NAME; }

protected:
    // Overriden protected SettingsLogger methods
    StringMap getComponentSettings() const override;
    SearchSettingsMap getSubComponentSettings() const override { return {}; }

private:
    /**
     * Caches the set of actions applicable for each location of the blank.
     *
     * Actions are generated according to the operator ordering currently in use.
     *
     * @param rows The number of rows in the puzzle.
     * @param cols The number of columns in the puzzle.
     */
    void setActionList();

    /**
     * Checks if the given action is applicable given the current location of the blank.
     *
     * @param action The action to check.
     * @param blank_loc The location of the blank.
     * @return If the given action is applicable given where the blank currently is.
     */
    bool isApplicableInLocation(const BlankSlide& action, int blank_loc) const;

    /**
     * Gets the identity of the tile being moved by the given action.
     *
     * Assumes the action is applicable in the given state.
     *
     * @param state The state in which the move is being applied.
     * @param action The action to apply.
     * @returns The identity of the tile being moved.
     */
    Tile getMovingTile(const SlidingTileState& state, const BlankSlide& action) const;

    int m_num_rows = 0;  ///< Number of rows in the puzzle.
    int m_num_cols = 0;  ///< Number of columns in the puzzle.

    SlidingTileCostType m_cost_type = SlidingTileCostType::unit;

    std::vector<std::vector<BlankSlide>> m_loc_actions;  ///< Caches the actions applicable for each blank location.
    std::vector<BlankSlide> m_ops_in_order;  ///< An ordering of all the possible actions.
    std::vector<double> m_tile_move_costs;  ///< Caches the cost of moving each tile.
};

#endif /* SLIDING_TILE_TRANSITIONS_H_ */
