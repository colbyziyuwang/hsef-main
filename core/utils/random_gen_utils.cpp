#include "random_gen_utils.h"

#include <random>

long getRandomNum(long min, long max, std::mt19937& gen) {
    std::uniform_int_distribution<long> dist(min, max);
    return dist(gen);
}

long getRandomNum(long max, std::mt19937& gen) {
    return getRandomNum(0, max, gen);
}

double getRandomProbability(std::mt19937& gen) {
    std::uniform_real_distribution<> dist(0, 1);
    return dist(gen);
}
