#ifndef SIGNED_PERMUTATION_HASH_FUNCTION_H_
#define SIGNED_PERMUTATION_HASH_FUNCTION_H_

#include "building_tools/hashing/state_hash_function.h"
#include "utils/combinatorics.h"


/**
 * A hash function for burnt pancake puzzle, based on the regular permutation hash function,
 * will consider the burnt side to generate a unique hash function
 */
template<class State_t>
class SignedPermutationHashFunction : public StateHashFunction<State_t, uint64_t> {
public:
    inline static const std::string CLASS_NAME = "SignedPermutationHashFunction";  ///< The name of the class. Defines this component's name

    uint64_t getHashValue(const State_t& state) const override;
    bool isPerfectHashFunction() const override { return true; }

    // Overriden public SettingsLogger methods
    std::string getName() const override { return CLASS_NAME; }

protected:
    // Overriden protected SettingsLogger methods
    StringMap getComponentSettings() const override { return {}; };
    SearchSettingsMap getSubComponentSettings() const override { return {}; }
};

template<class State_t>
inline uint64_t SignedPermutationHashFunction<State_t>::getHashValue(const State_t& state) const {
    assert(state.m_permutation.size() <= 16);

    std::vector<int> permutation = state.m_permutation;
    uint64_t signed_ranking = getBitVectorRanking(permutation);
    auto max_unsigned_ranking = static_cast<uint16_t>(get64BitFactorial(static_cast<unsigned>(permutation.size())));

    convertPermutationState(permutation);
    uint64_t unsigned_ranking = getPermutationRank(permutation);

    return signed_ranking * max_unsigned_ranking + unsigned_ranking;
}

#endif  //SIGNED_PERMUTATION_HASH_FUNCTION_H_