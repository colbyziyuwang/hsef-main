#ifndef K_ARY_TREE_TRANSITIONS_H_
#define K_ARY_TREE_TRANSITIONS_H_

#include "k_ary_tree_action.h"
#include "k_ary_tree_state.h"
#include "search_basics/transition_system.h"

#include <iostream>
#include <numeric>

/**
 * Defines the transition system for the k-ary tree domain.
 */
class KAryTreeTransitions : public TransitionSystem<KAryTreeState, KAryTreeAction> {
public:
    inline static const std::string CLASS_NAME = "KAryTransitions";  ///< The name of the class. Defines this component's name

    /**
     * Constructs a k-ary tree transition system with the given number of children.
     *
     * @param k The number of children each node can have.
     */
    explicit KAryTreeTransitions(int k);

    /**
     * Default destructor.
     */
    ~KAryTreeTransitions() override = default;

    // Overridden public TransitionSystem methods
    std::vector<KAryTreeAction> getActions(const KAryTreeState& state) const override;
    bool isApplicable(const KAryTreeState& state, const KAryTreeAction& action) const override;
    double getActionCost(const KAryTreeState& state, const KAryTreeAction& action) const override;
    void applyAction(KAryTreeState& state, const KAryTreeAction& action) const override;
    std::optional<KAryTreeAction> getInverse(const KAryTreeState& state, const KAryTreeAction& action) const override;
    bool isValidState(const KAryTreeState& state) const override;


    // Overridden public SettingsLogger methods
    std::string getName() const override { return CLASS_NAME; }

protected:
    // Overridden protected SettingsLogger methods
    StringMap getComponentSettings() const override;
    SearchSettingsMap getSubComponentSettings() const override { return {}; }

private:
    int m_k;  ///< Number of children each node can have.
};

#endif /* K_ARY_TREE_TRANSITIONS_H_ */
