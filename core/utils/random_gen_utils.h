/**
 * Defines functions for random number generation.
 *
 * @todo Currently assumes that the only random number generator being used is the 32 bit Mersenne Twister.
 *
 * @file random_gen_utils.h
 */
#ifndef RANDOM_GEN_UTILS_H_
#define RANDOM_GEN_UTILS_H_

#include <algorithm>  // need for std::shuffle
#include <random>
#include <vector>


/**
 * Returns a random number uniformly selected between the given parameter values, inclusive.
 *
 * @param min The minimum value that can be returned.
 * @param max The maximum value that can be returned.
 * @param gen The random number generator.
 * @return Random number between the given parameter values, inclusive.
 */
long getRandomNum(long min, long max, std::mt19937& gen);

/**
 * Returns a random number uniformly selected from between 0 and the given parameter value, inclusive.
 *
 * @param max The maximum random number to return.
 * @param gen The random number generator.
 * @return Random number between 0 and the given parameter value, inclusive.
 */
long getRandomNum(long max, std::mt19937& gen);

/**
 * Returns a random probability value using the given random number generator.
 * This means it returns a double that is distributed uniformly at random in the interval [0, 1).
 *
 * @param gen The random number generator.
 * @return Returns a random number in the interval [0, 1).
 */
double getRandomProbability(std::mt19937& gen);

/**
 * Randomly reorders the elements in the given vector.
 *
 * @param vec The vector to randomly reorder.
 * @param gen The random number generator.
 */
template<class Element_t>
void randomlyReorderVector(std::vector<Element_t>& vec, std::mt19937& gen);

// Templated function implementations
template<class Element_t>
void randomlyReorderVector(std::vector<Element_t>& vec, std::mt19937& gen) {
    shuffle(vec.begin(), vec.end(), gen);
}

#endif /* RANDOM_GEN_UTILS_H_ */
