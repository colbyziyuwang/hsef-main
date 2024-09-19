#ifndef BURNT_PANCAKE_TRANSITIONS_H_
#define BURNT_PANCAKE_TRANSITIONS_H_

#include "burnt_pancake_state.h"
#include "environments/pancake_puzzle/pancake_action.h"
#include "environments/pancake_puzzle/pancake_transitions.h"
#include "search_basics/transition_system.h"

#include "logging/logging_terms.h"
#include "logging/search_component_settings.h"
#include <optional>
#include <string>
#include <vector>

/**
 * Defines the transitions for the pancake puzzle.
 *
 * @class BurntPancakeTransitions
 */
class BurntPancakeTransitions : public TransitionSystem<BurntPancakeState, NumToFlip> {
public:
    inline static const std::string CLASS_NAME = "SlidingTileTransitions";  ///< The name of the class. Defines this component's name

    /**
     * A constructor for a pancake puzzle transition system.
     * 
     * @param size Unsigned integer representing the size of the pancake puzzle transition environment.
     * @param cost_type The action cost type of the pancake puzzle
     */
    explicit BurntPancakeTransitions(int size, PancakePuzzleCostType cost_type = PancakePuzzleCostType::unit);

    /**
     * Default destructor.
     */
    ~BurntPancakeTransitions() override = default;

    // Overriden public TransitionSystem methods
    std::vector<NumToFlip> getActions(const BurntPancakeState& state) const override;
    bool isApplicable(const BurntPancakeState& state, const NumToFlip& action) const override;
    double getActionCost(const BurntPancakeState& state, const NumToFlip& action) const override;
    void applyAction(BurntPancakeState& state, const NumToFlip& action) const override;
    std::optional<NumToFlip> getInverse(const BurntPancakeState& state, const NumToFlip& action) const override;
    bool isValidState(const BurntPancakeState& state) const override;

    /**
     * Sets the action costs to one of the standard types.
     *
     * @param cost_type The cost type for the transitions.
     */
    void setTileCostType(PancakePuzzleCostType cost_type);

    // Overriden public SettingsLogger methods
    std::string getName() const override { return CLASS_NAME; }

protected:
    // Overriden protected SettingsLogger methods
    StringMap getComponentSettings() const override;
    SearchSettingsMap getSubComponentSettings() const override { return {}; }

private:
    int m_stack_size;  ///< Size of the pancake puzzle.
    PancakePuzzleCostType m_cost_type;  ///< The action cost type of the pancake puzzle.
};

#endif /* BURNT_PANCAKE_TRANSITIONS_H_ */
