#ifndef PANCAKE_TRANSITIONS_H_
#define PANCAKE_TRANSITIONS_H_

#include "logging/logging_terms.h"
#include "logging/search_component_settings.h"
#include "pancake_action.h"
#include "pancake_state.h"
#include "search_basics/transition_system.h"

#include <cstdint>
#include <optional>
#include <string>
#include <vector>

/**
 * Defines the possible cost types for actions in the pancake puzzle domain.
 *
 * Below is the meaning of each of the values:
 *
 * unit - all actions have a cost of 1.\n
 * heavy - cost of the action that flips a prefix (V[1] ...V[i + 1]) is max(V[1], V[i + 1]).\n
 *
 * @class PancakePuzzleCostType
 */
enum class PancakePuzzleCostType : std::uint8_t {
    unit,
    heavy
};

/**
 * Defines the transitions for the pancake puzzle.
 *
 * @class PancakeTransitions
 */
class PancakeTransitions : public TransitionSystem<PancakeState, NumToFlip> {
public:
    inline static const std::string CLASS_NAME = "PancakeTransitions";  ///< The name of the class. Defines this component's name

    /**
     * A constructor for a pancake puzzle transition system.
     * 
     * @param size Unsigned integer representing the size of the pancake puzzle transition environment.
     * @param cost_type The action cost type of the pancake puzzle
     */
    explicit PancakeTransitions(int size, PancakePuzzleCostType cost_type = PancakePuzzleCostType::unit);

    /**
     * Default destructor.
     */
    ~PancakeTransitions() override = default;

    // Overriden public TransitionSystem methods
    std::vector<NumToFlip> getActions(const PancakeState& state) const override;
    bool isApplicable(const PancakeState& state, const NumToFlip& action) const override;
    double getActionCost(const PancakeState& state, const NumToFlip& action) const override;
    void applyAction(PancakeState& state, const NumToFlip& action) const override;
    std::optional<NumToFlip> getInverse(const PancakeState& state, const NumToFlip& action) const override;
    bool isValidState(const PancakeState& state) const override;

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

#endif /* PANCAKE_TRANSITIONS_H_ */
