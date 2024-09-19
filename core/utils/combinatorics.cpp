#include "combinatorics.h"
#include "random_gen_utils.h"
#include "string_utils.h"
#include <cassert>
#include <fstream>
#include <iostream>
#include <string>

using std::cerr;
using std::ifstream;
using std::stoi;
using std::string;
using std::vector;

void convertPermutationState(std::vector<int>& permutation) {
    for (int& value : permutation) {
        if (value < 0) {
            value *= -1;
        }
        value--;
    }
}

uint64_t getBitVectorRanking(const std::vector<int>& permutation) {
    uint64_t sign_bit = 0;
    uint64_t twos = 1;
    for (int i = static_cast<int>(permutation.size()) - 1; i >= 0; i--) {
        if (permutation[i] < 0) {
            sign_bit += twos;
        }
        twos *= 2;
    }
    return sign_bit;
}

/**
 * Helper function for get_next combo.
 *
 * Given a combination, a position, and whether we are at a reset, finds the next combo if one
 * exists. This means incrementing the element at the given position if possible, and then resetting
 * the remaining values to the smallest values left. If its not possible, it returns false.
 *
 * @param combo The current combo which is updated to get the next combo.
 * @param max_num The maximum element value.
 * @param pos The current position.
 * @param reset_rest Whether the combo should be reset starting from the given position.
 * @return If we successfully found the next combination.
 */
bool getNextComboHelper(vector<unsigned>& combo, unsigned max_num, unsigned pos, bool reset_rest);

static const vector<uint64_t> FACTORIAL_TABLE = {
          1LL, 1LL, 2LL, 6LL, 24LL, 120LL, 720LL, 5040LL, 40320LL, 362880LL, 3628800LL, 39916800LL,
          479001600LL, 6227020800LL, 87178291200LL, 1307674368000LL, 20922789888000LL,
          355687428096000LL, 6402373705728000LL, 121645100408832000LL, 2432902008176640000LL};

bool getNextCombo(vector<unsigned>& combo, unsigned combo_size, unsigned num_elems, bool first_combo) {
    assert(num_elems >= combo_size);

    if (first_combo || combo.size() != combo_size) {  // need to initialize first combo
        combo.resize(combo_size);

        for (unsigned i = 0; i < combo_size; i++) {
            combo[i] = i;
        }
        return true;
    }
    assert(num_elems >= combo.size() - 1);

    return getNextComboHelper(combo, num_elems - 1, 0, false);  // call helper function to get next combo
}

bool getNextComboHelper(vector<unsigned>& combo, unsigned max_num, unsigned pos, bool reset_rest) {
    assert(max_num >= combo.size() - 1);

    if (reset_rest) {  // need to reset rest of the combo to smallest values possible starting from current position
        if (pos == 0) {  // if this is the first position, reset the whole combo
            combo[0] = 0;
            return getNextComboHelper(combo, max_num, pos + 1, true);
        }

        assert(combo[pos - 1] + combo.size() - pos <= max_num);  // make sure enough positions for remaining elements

        combo[pos] = combo[pos - 1] + 1;  // assign the smallest remaining element

        if (pos == combo.size() - 1) {  // if finished the combo, return it was successful
            return true;
        }

        return getNextComboHelper(combo, max_num, pos + 1, true);  // move to resetting next value
    }

    // if have hit a number that is too high, must backtrack and replace
    if (combo[pos] + combo.size() - pos - 1 == max_num) {  // have run out of elements, and must backtrack
        return false;
    } else if (pos == combo.size() - 1) {  // at last position and larger elements remaining
        combo[pos]++;
        return true;
    }

    if (!getNextComboHelper(combo, max_num, pos + 1, false)) {  // try to increase elements in remainder of combo
        combo[pos]++;  // if can't, have to update the current one first
        return getNextComboHelper(combo, max_num, pos + 1, true);  // reset remainder accordingly
    }

    return true;
}

uint64_t getPermutationRank(const std::vector<int>& permutation) {
    assert(permutation.size() <= 64);
    std::vector<int> perm = permutation;
    auto num_left = static_cast<unsigned>(perm.size());
    uint64_t hash_value = 0;
    for (unsigned i = 0; i < perm.size(); i++) {
        hash_value += perm[i] * get64BitFactorial(num_left - 1);
        num_left--;
        for (unsigned j = i + 1; j < perm.size(); j++) {
            if (perm[j] > perm[i]) {
                perm[j]--;
            }
        }
    }
    return hash_value;
}

vector<int> getRandomPermutation(unsigned size, std::mt19937& gen) {
    vector<int> permutation(size);

    for (int i = 0; i < static_cast<int>(size); i++) {
        permutation[i] = i;
    }

    randomlyReorderVector<int>(permutation, gen);
    return permutation;
}

uint64_t get64BitFactorial(unsigned num) {
    if (num > 20) {
        return static_cast<uint64_t>(-1);
    }
    return FACTORIAL_TABLE[num];
}

uint64_t get64BitNUpperK(unsigned num_elems, unsigned combo_size) {
    if (num_elems <= combo_size) {
        return 1;
    }
    if (num_elems <= 20) {
        return FACTORIAL_TABLE[num_elems] / FACTORIAL_TABLE[combo_size];
    }

    uint64_t value = 1;
    for (unsigned i = num_elems; i > combo_size; i--) {
        value *= static_cast<uint64_t>(i);
    }

    return value;
}

std::vector<std::vector<int>> readPermutations(std::istream& perm_stream, bool is_signed) {

    std::vector<std::vector<int>> permutations;

    string new_line;
    vector<string> tokens;
    vector<int> perm;
    int line_count = 1;

    while (getline(perm_stream, new_line)) {
        tokens = split(new_line, ' ');

        if (tokens.empty()) {
            continue;
        }

        perm.clear();
        for (auto& token : tokens) {
            perm.push_back(stoi(token));
        }

        if ((is_signed && isValidSignedPermutation(perm)) || (!is_signed && isValidPermutation(perm))) {
            permutations.push_back(perm);
        } else {
            cerr << "Invalid permutation on line " << line_count << std::endl;
        }
        line_count++;
    }

    return permutations;
}

bool isValidPermutation(const vector<int>& to_check) {
    vector<bool> value_seen(to_check.size(), false);

    for (auto value : to_check) {
        if (value < 0 || value >= static_cast<int>(to_check.size()) || value_seen[value]) {
            return false;
        }
        value_seen[value] = true;
    }
    return true;
}

bool isValidSignedPermutation(const vector<int>& to_check) {
    vector<bool> value_seen(to_check.size(), false);

    for (auto value : to_check) {
        int abs_value = abs(value);
        if (abs_value == 0 || abs_value > static_cast<int>(to_check.size()) || value_seen[abs_value - 1]) {
            return false;
        }
        value_seen[abs_value - 1] = true;
    }
    return true;
}
