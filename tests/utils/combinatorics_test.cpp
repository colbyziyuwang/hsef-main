#include <algorithm>
#include <gtest/gtest.h>

#include "utils/combinatorics.h"

/**
 * Helper function to check if two vectors are equal
 *
 * @param vec1 The first vector
 * @param vec2 The second vector
 */
void checkVectorEquals(std::vector<unsigned>& vec1, std::vector<unsigned>& vec2) {
    ASSERT_EQ(vec1.size(), vec2.size());
    for (unsigned i = 0; i < vec1.size(); ++i) {
        EXPECT_EQ(vec1[i], vec2[i]) << "Vectors tokens and expected_tokens differ at index " << i;
    }
}

/**
 * Enter some arrays with negative numbers and 
 * see if you can convert them into arrays containing 
 * only positive numbers and the number will minus one.
 */
TEST(CombinatoricsTests, convertPermutationStateTest) {
    std::vector<int> nums1 = {-1, 2, 3, 4};
    std::vector<int> nums2 = {-1, 4, -3, 2};
    std::vector<int> nums3 = {6, 3, -2, -1, 4, -5};

    std::vector<int> expected_arr1 = {0, 1, 2, 3};
    std::vector<int> expected_arr2 = {0, 3, 2, 1};
    std::vector<int> expected_arr3 = {5, 2, 1, 0, 3, 4};

    convertPermutationState(nums1);
    convertPermutationState(nums2);
    convertPermutationState(nums3);

    ASSERT_EQ(nums1, expected_arr1);
    ASSERT_EQ(nums2, expected_arr2);
    ASSERT_EQ(nums3, expected_arr3);
}

/**
 * Input an array containing negative and positive value, the function will
 * treat positive as 0 and negative value as 1. Check if the binary
 * represented by it can be successfully converted to decimal output.
 */
TEST(CombinatoricsTests, getBitVectorRankingTest) {
    std::vector<int> nums1 = {1, 2, 3};
    std::vector<int> nums2 = {2, 3, 4, -1};
    std::vector<int> nums3 = {3, 4, -2, 1};
    std::vector<int> nums4 = {-1, -2, -3, -4, -5};

    ASSERT_EQ(getBitVectorRanking(nums1), 0);
    ASSERT_EQ(getBitVectorRanking(nums2), 1);
    ASSERT_EQ(getBitVectorRanking(nums3), 2);
    ASSERT_EQ(getBitVectorRanking(nums4), 31);
}

/**
 * Generates the first six get_next_combos for k = 3 and n = 5
 * and checks if they are the expected values.
 */
TEST(CombinatoricsTests, getNextComboTest) {
    std::vector<unsigned> nums;

    std::vector<unsigned> expected_nums{0, 1, 2};
    ASSERT_TRUE(getNextCombo(nums, 3, 5));
    checkVectorEquals(nums, expected_nums);

    expected_nums = {0, 1, 3};
    ASSERT_TRUE(getNextCombo(nums, 3, 5));
    checkVectorEquals(nums, expected_nums);

    expected_nums = {0, 1, 4};
    ASSERT_TRUE(getNextCombo(nums, 3, 5));
    checkVectorEquals(nums, expected_nums);

    expected_nums = {0, 2, 3};
    ASSERT_TRUE(getNextCombo(nums, 3, 5));
    checkVectorEquals(nums, expected_nums);

    expected_nums = {0, 2, 4};
    ASSERT_TRUE(getNextCombo(nums, 3, 5));
    checkVectorEquals(nums, expected_nums);

    expected_nums = {0, 3, 4};
    ASSERT_TRUE(getNextCombo(nums, 3, 5));
    checkVectorEquals(nums, expected_nums);
}

/**
 * Test getPermutationRank function can return unique hash value
 */
TEST(CombinatoricsTests, getPermutationRankTest) {
    std::vector<int> nums1 = {0, 1, 2, 3};
    std::vector<int> nums2 = {3, 2, 1, 0};
    std::vector<int> nums3 = {2, 1, 3, 0};

    ASSERT_EQ(getPermutationRank(nums1), 0);
    ASSERT_EQ(getPermutationRank(nums2), 23);
    ASSERT_EQ(getPermutationRank(nums3), 15);
}

/**
 * Generates two random permutations and checks if they are both different in order.
 * This test might be a bit weak.
 */
TEST(CombinatoricsTests, getRandPermutationTest) {
    auto seed = std::random_device{}();
    std::mt19937 gen(seed);

    std::vector<int> shuffled_nums;
    shuffled_nums = getRandomPermutation(100, gen);

    for (int num = 0; num < 100; num++) {
        bool num_is_in = (std::find(shuffled_nums.begin(), shuffled_nums.end(), num) != shuffled_nums.end());
        ASSERT_EQ(num_is_in, true) << "Permutation does not have a value (" << num << "). Seed: "
                                   << seed;
    }
}

/**
 * Tests get64BitFactorial to see if it returns the correct value of n!
 */
TEST(CombinatoricsTests, get64BitFactorialTest) {
    ASSERT_EQ(get64BitFactorial(0), 1);
    ASSERT_EQ(get64BitFactorial(1), 1);
    ASSERT_EQ(get64BitFactorial(2), 2);
    ASSERT_EQ(get64BitFactorial(5), 120);
    ASSERT_EQ(get64BitFactorial(14), static_cast<uint64_t>(87178291200));
}

/**
 * Tests get64BitnUpperk to see if it returns the correct value of n!/k!
 */
TEST(CombinatoricsTests, get64BitNUpperkTest) {
    ASSERT_EQ(get64BitNUpperK(6, 0), 720);
    ASSERT_EQ(get64BitNUpperK(3, 1), 6);
    ASSERT_EQ(get64BitNUpperK(8, 2), 20160);
    ASSERT_EQ(get64BitNUpperK(5, 3), 20);
    ASSERT_EQ(get64BitNUpperK(14, 9), 240240);
}

/**
 * Tests that isValidPermutation works on both valid and invalid permutations.
 */
TEST(CombinatoricsTests, isValidPermutationTest) {
    std::vector<int> good_perm1 = {3, 0, 2, 1};
    std::vector<int> good_perm2 = {0};
    std::vector<int> good_perm3 = {5, 2, 0, 1, 4, 3};

    ASSERT_TRUE(isValidPermutation(good_perm1));
    ASSERT_TRUE(isValidPermutation(good_perm2));
    ASSERT_TRUE(isValidPermutation(good_perm3));

    std::vector<int> bad_perm1 = {3, 0, -2, 1};
    std::vector<int> bad_perm2 = {1};
    std::vector<int> bad_perm3 = {4, 2, 0, 1, 4, 3};
    std::vector<int> bad_perm4 = {5, 2, 0, 1, 6, 3};

    ASSERT_FALSE(isValidPermutation(bad_perm1));
    ASSERT_FALSE(isValidPermutation(bad_perm2));
    ASSERT_FALSE(isValidPermutation(bad_perm3));
    ASSERT_FALSE(isValidPermutation(bad_perm4));
}

/**
 * Tests that isValidSignedPermutation works on both valid and invalid permutations.
 */
TEST(CombinatoricsTests, testIsSignedValidPermutation) {
    std::vector<int> good_perm1 = {-3, 4, -2, 1};
    std::vector<int> good_perm2 = {1};
    std::vector<int> good_perm3 = {-1};
    std::vector<int> good_perm4 = {5, -2, 6, 1, 4, -3};
    std::vector<int> good_perm5 = {-5, -2, -6, -1, -4, -3};

    ASSERT_TRUE(isValidSignedPermutation(good_perm1));
    ASSERT_TRUE(isValidSignedPermutation(good_perm2));
    ASSERT_TRUE(isValidSignedPermutation(good_perm3));
    ASSERT_TRUE(isValidSignedPermutation(good_perm4));
    ASSERT_TRUE(isValidSignedPermutation(good_perm5));

    std::vector<int> bad_perm1 = {3, 0, -2, 1};
    std::vector<int> bad_perm2 = {0};
    std::vector<int> bad_perm3 = {2};
    std::vector<int> bad_perm4 = {4, 2, 5, 1, 4, 3};
    std::vector<int> bad_perm5 = {5, 2, -7, 1, -4, -3};
    std::vector<int> bad_perm6 = {6, 2, -7, 1, -4, -3};

    ASSERT_FALSE(isValidSignedPermutation(bad_perm1));
    ASSERT_FALSE(isValidSignedPermutation(bad_perm2));
    ASSERT_FALSE(isValidSignedPermutation(bad_perm3));
    ASSERT_FALSE(isValidSignedPermutation(bad_perm4));
    ASSERT_FALSE(isValidSignedPermutation(bad_perm5));
    ASSERT_FALSE(isValidSignedPermutation(bad_perm6));
}

/**
 * Tests that read permutations works as expected.
 */
TEST(CombinatoricsTests, readPermutationsTest) {
    std::stringstream empty_stream("");

    // Check empty file
    auto permutations = readPermutations(empty_stream, false);
    ASSERT_EQ(permutations.size(), 0);

    // Check unsigned permutations with empty line
    std::stringstream unsigned_perms("0 1 2 3\n2 5 3 1 0 4\n2 3 1\n0 2 1");
    permutations = readPermutations(unsigned_perms, false);
    ASSERT_EQ(permutations.size(), 3);
    ASSERT_EQ(permutations[0], std::vector<int>({0, 1, 2, 3}));
    ASSERT_EQ(permutations[1], std::vector<int>({2, 5, 3, 1, 0, 4}));
    ASSERT_EQ(permutations[2], std::vector<int>({0, 2, 1}));

    std::stringstream signed_perms("4 1 2 -3\n2 5 -3 1 -6 4\n-2 4 1\n3 2 1");
    permutations = readPermutations(signed_perms, true);
    ASSERT_EQ(permutations.size(), 3);
    ASSERT_EQ(permutations[0], std::vector<int>({4, 1, 2, -3}));
    ASSERT_EQ(permutations[1], std::vector<int>({2, 5, -3, 1, -6, 4}));
    ASSERT_EQ(permutations[2], std::vector<int>({3, 2, 1}));
}