#include <gtest/gtest.h>

#include "utils/floating_point_utils.h"

/**
 * Tests fpLess to see if the first argument is actually smaller than the second
 */
TEST(FloatingPointTests, fPLessTest) {
    EXPECT_EQ(fpLess(1.5, 2.3), true);
    EXPECT_EQ(fpLess(1.5, 0.3), false);
    EXPECT_EQ(fpLess(1.5, 1.5), false);
    EXPECT_EQ(fpLess(69.666, 69.7), true);
}

/**
 * Tests fpGreater to see if the first argument is actually greater than the second
 */
TEST(FloatingPointTests, fPGreaterTest) {
    EXPECT_EQ(fpGreater(1.5, 2.3), false);
    EXPECT_EQ(fpGreater(1.5, 0.3), true);
    EXPECT_EQ(fpGreater(1.5, 1.5), false);
    EXPECT_EQ(fpGreater(69.666, 69.7), false);
}

/**
 * Tests fpEqual to see if the first argument is actually equal to the second
 */
TEST(FloatingPointTests, fPEqualTest) {
    EXPECT_EQ(fpEqual(1.5, 2.3), false);
    EXPECT_EQ(fpEqual(1.5, 0.3), false);
    EXPECT_EQ(fpEqual(1.5, 1.5), true);
    EXPECT_EQ(fpEqual(69.666, 69.7), false);
    EXPECT_EQ(fpEqual(69.666, 69.666), true);
    EXPECT_EQ(fpEqual(69.6666, 69.666), false);
    EXPECT_EQ(fpEqual(69.6666, 69.6666), true);
    EXPECT_EQ(fpEqual(69.66666, 69.66666), true);
    EXPECT_EQ(fpEqual(69.666666, 69.666666), true);
    EXPECT_EQ(fpEqual(69.666666, 69.6666671), false);
    EXPECT_EQ(fpEqual(69.666666, 69.6666651), true);
}

/**
 * Tests fpMin to see if the returned value is actually the minimum between the two
 */
TEST(FloatingPointTests, fPMinTest) {
    EXPECT_EQ(fpMin(1.5, 2.3), 1.5);
    EXPECT_EQ(fpMin(1.5, 0.3), 0.3);
    EXPECT_EQ(fpMin(1.5, 1.5), 1.5);
    EXPECT_EQ(fpMin(69.666, 69.7), 69.666);
    EXPECT_EQ(fpMin(69.666666, 69.666666), 69.666666);
    EXPECT_EQ(fpMin(69.666666, 69.666667), 69.666666);
    EXPECT_EQ(fpMin(69.666666, 69.666665), 69.666665);
}

/**
 * Tests fpMax to see if the returned value is actually the maximum between the two
 */
TEST(FloatingPointTests, fPMaxTest) {
    EXPECT_EQ(fpMax(1.5, 2.3), 2.3);
    EXPECT_EQ(fpMax(1.5, 0.3), 1.5);
    EXPECT_EQ(fpMax(1.5, 1.5), 1.5);
    EXPECT_EQ(fpMax(69.666, 69.7), 69.7);
    EXPECT_EQ(fpMax(69.666666, 69.666666), 69.666666);
    EXPECT_EQ(fpMax(69.6666, 69.6667), 69.6667);
    EXPECT_EQ(fpMax(69.666666, 69.666665), 69.666666);
}