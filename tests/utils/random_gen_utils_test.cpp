#include <gtest/gtest.h>
#include <unordered_map>

#include "utils/random_gen_utils.h"

/**
 * Tests getRandomNum where we only specify the max value.
 * Values should be between 0 and max.
 */
TEST(RandomGenTests, getRandomNumMaxTest) {
    auto seed = std::random_device{}();
    std::mt19937 gen(seed);

    // Test #1: <= 1000000
    long max = 1000000;
    for (int i = 0; i < 100; i++) {
        long num = getRandomNum(max, gen);
        EXPECT_GE(num, 0) << "Random number generated is less than specified minimum value. Seed: " << seed;
        EXPECT_LE(num, max) << "Random number generated is greater than specified maximum value. Seed: " << seed;
    }

    // Test #2: <= 500000
    max = 500000;
    for (int i = 0; i < 100; i++) {
        long num = getRandomNum(max, gen);
        EXPECT_GE(num, 0) << "Random number generated is less than specified minimum value. Seed: " << seed;
        EXPECT_LE(num, max) << "Random number generated is greater than specified maximum value. Seed: " << seed;
    }

    // Test #3: <= 100
    max = 100;
    for (int i = 0; i < 100; i++) {
        long num = getRandomNum(max, gen);
        EXPECT_GE(num, 0) << "Random number generated is less than specified minimum value. Seed: " << seed;
        EXPECT_LE(num, max) << "Random number generated is greater than specified maximum value. Seed: " << seed;
    }
}

/**
 * Tests getRandomNum where we only specify the min and max value.
 * Values should be between min and max.
 */
TEST(RandomGenTests, getRandomNumMinMaxTest) {
    auto seed = std::random_device{}();
    std::mt19937 gen(seed);

    // Test #1: 500000 <= num <= 1000000
    long min = 500000, max = 1000000;
    for (int i = 0; i < 100; i++) {
        long num = getRandomNum(min, max, gen);
        EXPECT_GE(num, min) << "Random number generated is less than specified minimum value. Seed: " << seed;
        EXPECT_LE(num, max) << "Random number generated is greater than specified maximum value. Seed: " << seed;
    }

    // Test #2: 100000 <= num <= 500000
    min = 100000, max = 500000;
    for (int i = 0; i < 100; i++) {
        long num = getRandomNum(min, max, gen);
        EXPECT_GE(num, min) << "Random number generated is less than specified minimum value. Seed: " << seed;
        EXPECT_LE(num, max) << "Random number generated is greater than specified maximum value. Seed: " << seed;
    }

    // Test #3: 500000 <= num <= 1000000
    min = 50, max = 100;
    for (int i = 0; i < 100; i++) {
        long num = getRandomNum(min, max, gen);
        EXPECT_GE(num, min) << "Random number generated is less than specified minimum value. Seed: " << seed;
        EXPECT_LE(num, max) << "Random number generated is greater than specified maximum value. Seed: " << seed;
    }
}

/**
 * Tests getRandomProbability.
 * Values should be between [0, 1).
 */
TEST(RandomGenTests, getRandomProbabilityTest) {
    auto seed = std::random_device{}();
    std::mt19937 gen(seed);

    // Test #1: 0 <= num < 1
    for (int i = 0; i < 100; i++) {
        double num = getRandomProbability(gen);
        EXPECT_GE(num, 0) << "Random number generated is less than specified minimum value. Seed: " << seed;
        EXPECT_LT(num, 1) << "Random number generated is greater than specified maximum value. Seed: " << seed;
    }
}

/**
 * Tests randomlyReorderVector.
 * Assumes (incorrectly) that shuffled vector should be in different order than original
 * and has all of the original values.
 */
TEST(RandomGenTests, randomlyReorderIntegerTest) {
    auto seed = std::random_device{}();
    std::mt19937 gen(seed);

    std::vector<int> nums{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    std::vector<int> shuffled_nums(nums.begin(), nums.end());

    // Remember all of the original values and their counts
    std::unordered_map<int, int> values;
    for (int num : nums) {
        // unordered_maps ints have a default value of 0
        values[num] += 1;
    }

    randomlyReorderVector(shuffled_nums, gen);

    // Check if shuffled vector contains all the original elements
    ASSERT_EQ(nums.size(), shuffled_nums.size()) << "Expected vectors to be same size. Seed: " << seed;
    for (int num : shuffled_nums) {
        values[num] -= 1;
    }

    // After decrementing, should be 0
    for (const auto& val : values) {
        ASSERT_EQ(val.second, 0);
    }
}