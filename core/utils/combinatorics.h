/**
 * A set of combinatorial functions.
 *
 * @todo Figure out proper way to get random numbers.
 * @todo Add something that checks to make sure read permutations are proper permutations.
 * @file combinatorics.h
 */
#ifndef COMBINATORICS_H_
#define COMBINATORICS_H_

#include <cstdint>
#include <cstdlib>
#include <random>
#include <string>
#include <vector>

/**
 * Checks if the given vector of integers is a valid permutation.
 *
 * @param to_check The vector to check
 * @return If the given vector is a valid permutation or not
 */
bool isValidPermutation(const std::vector<int>& to_check);

/**
 * Checks if the given vector of integers is a valid signed permutation.
 *
 * @param to_check The vector to check
 * @return If the given vector is a valid signed permutation or not
 */
bool isValidSignedPermutation(const std::vector<int>& to_check);

/**
* Given a signed permutation of length n (ie. a vector of integers from 1 to n, each of 
* which be positive or negative), converts the permutation to a standard permutation of 
* size n (ie. a vector of positive integers from 0 to n-1). Does so by dropping the sign 
* and shifting each value by 1
* 
* @param permutation The input vector of integer number
*/
void convertPermutationState(std::vector<int>& permutation);

/**
* Calculates a ranking of bit vector by converting the vector to a binary number (negative 
* values count as "on", positive values are "off"). Works on any vector of integers by 
* ignoring entry magnitude. Can be used to generate a hash value that is unique for each vector
* 
* @param permutation The input vector of integer number
*/
uint64_t getBitVectorRanking(const std::vector<int>& permutation);

/**
 * Given a set (or combination) of natural numbers, generates the next combination in the 
 * lexicographic ordering.
 *
 * This function allows the user to iterate through all combinations of sets of k natural 
 * numbers. Given such
 * a combination as a vector of sorted numbers, it generates the next combination 
 * if one exists and returns true. The combination is also returned as a 
 * sorted vector. If no such combination exists, it returns false and does not change the 
 * combination.
 *
 * Note that the first vector in the ordering is [0, 1, ..., k - 1] 
 * and the last is [n - k, n - 2, n - 1].
 *
 * The first vector in the ordering is generated if the given combination does 
 * not have the correct number of elements or the optional flag is set to true. 
 * Otherwise, the functions assumes it is given a sorted vector representing 
 * a valid combination of k of the first n natural numbers.
 *
 * @param combo The last combination generated.
 * @param combo_size The size of each combination.
 * @param num_elems The size of the set of natural numbers to select from.
 * @param first_combo Whether to generate the first combination.
 * @return If a new combination has been generated, or if it has run out of combinations.
 */
bool getNextCombo(std::vector<unsigned>& combo, unsigned combo_size, unsigned num_elems, bool first_combo = false);

/**
* Given a permutation of length n (ie. a vector of positive integers from 0 to n -1), 
* calculates a ranking of the permutation such that each unique permutation has a unique 
* value. This function can be used to generate a hash value for a permutation problem.
* 
* @param permutation The input vector, use integer number to stand for the state
*/
uint64_t getPermutationRank(const std::vector<int>& permutation);

/**
 * Returns a random permutation of a subset of the natural numbers using the 
 * given random number generator.
 *
 * @param size The size of the permutation to generate
 * @param rand_generator The random number generator
 * @return The random permutation that was generated
 */
std::vector<int> getRandomPermutation(unsigned size, std::mt19937& rand_generator);

/**
 * Returns n! for the given value of n.
 *
 * If n is too large, it just returns the highest value stored in 64 bits.
 *
 * @param num The value to take the factorial of.
 * @return The factorial value of the given value.
 */
uint64_t get64BitFactorial(unsigned num);

/**
 * Returns the value of n!/k! for the given integers.
 *
 * No guarantees on what happens if there is an overflow.
 * @todo Figure out what should happen when an overflow happens.
 *
 * @param num_elems The denominator of the n!/k! term.
 * @param combo_size The numerator of the n!/k! term.
 * @return The value of n!/k! for the given integers.
 */
uint64_t get64BitNUpperK(unsigned num_elems, unsigned combo_size);

/**
 * Reads in a vector of permutations from the stream. Assumes each permutation is given on a separate line, with the
 * entries separated by a single blank space.
 *
 * @param perm_stream The stream to read permutations from
 * @param is_signed Is the permutations signed or not
 * @return The vector of read permutations
 */
std::vector<std::vector<int>> readPermutations(std::istream& perm_stream, bool is_signed);

#endif /* COMBINATORICS_H_ */
