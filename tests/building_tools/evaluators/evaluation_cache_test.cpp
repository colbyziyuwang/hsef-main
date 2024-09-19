#include <gtest/gtest.h>

#include "building_tools/evaluators/evaluation_cache.h"

/**
 * Tests that basic add and get functionality works correctly when values are added one after
 * another
 */
TEST(EvaluationCacheTests, addOneAfterAnotherTest) {
    EvaluationCache cache;

    ASSERT_EQ(cache.size(), 0);

    // Add first set of values
    cache.setValues(0, 1.0, true);

    ASSERT_EQ(cache.size(), 1);
    ASSERT_EQ(cache.getEvaluation(0), 1.0);
    ASSERT_EQ(cache.getIsDeadEnd(0), true);

    // Add second set of values
    cache.setEvaluation(1, 2.0);
    cache.setIsDeadEnd(1, false);

    ASSERT_EQ(cache.size(), 2);
    ASSERT_EQ(cache.getEvaluation(0), 1.0);
    ASSERT_EQ(cache.getIsDeadEnd(0), true);
    ASSERT_EQ(cache.getEvaluation(1), 2.0);
    ASSERT_EQ(cache.getIsDeadEnd(1), false);

    // Add third set of values
    cache.setValues(2, 7.8, false);

    ASSERT_EQ(cache.size(), 3);
    ASSERT_EQ(cache.getEvaluation(0), 1.0);
    ASSERT_EQ(cache.getIsDeadEnd(0), true);
    ASSERT_EQ(cache.getEvaluation(1), 2.0);
    ASSERT_EQ(cache.getIsDeadEnd(1), false);
    ASSERT_EQ(cache.getEvaluation(2), 7.8);
    ASSERT_EQ(cache.getIsDeadEnd(2), false);
}

/**
 * Tests that resetting previously set values works correctly. Also checks that clearCache works.
 */
TEST(EvaluationCacheTests, resetValuesTest) {
    EvaluationCache cache;

    cache.setValues(0, 1.0, true);
    cache.setValues(1, 2.0, false);
    cache.setValues(2, 7.8, false);

    cache.setEvaluation(1, 99.0);
    cache.setIsDeadEnd(0, false);

    ASSERT_EQ(cache.size(), 3);
    ASSERT_EQ(cache.getEvaluation(0), 1.0);
    ASSERT_EQ(cache.getIsDeadEnd(0), false);
    ASSERT_EQ(cache.getEvaluation(1), 99.0);
    ASSERT_EQ(cache.getIsDeadEnd(1), false);
    ASSERT_EQ(cache.getEvaluation(2), 7.8);
    ASSERT_EQ(cache.getIsDeadEnd(2), false);

    cache.clearCache();
    ASSERT_EQ(cache.size(), 0);
}

/**
 * Ensures that default values are set correctly when some explicit set calls are not made.
 */
TEST(EvaluationCacheTests, defaultsTest) {
    EvaluationCache cache;

    cache.setEvaluation(0, 1.0);
    cache.setIsDeadEnd(0, true);
    cache.setEvaluation(1, 2.0);
    cache.setIsDeadEnd(1, false);
    cache.setEvaluation(2, 7.8);
    cache.setIsDeadEnd(2, false);

    // Test that isDeadEnd is set correctly when only evaluation is set for new node
    cache.setEvaluation(3, 99.0);
    ASSERT_EQ(cache.size(), 4);
    ASSERT_EQ(cache.getEvaluation(3), 99.0);
    ASSERT_EQ(cache.getIsDeadEnd(3), false);

    // Test that evaluation is set correctly when only isDeadEnd is set for new node
    cache.setIsDeadEnd(4, true);
    ASSERT_EQ(cache.size(), 5);
    ASSERT_EQ(cache.getEvaluation(4), 0.0);
    ASSERT_EQ(cache.getIsDeadEnd(4), true);

    // Test that all defaults set when setEvaluation is called for "much" larger node id
    cache.setEvaluation(8, 56.0);
    ASSERT_EQ(cache.getEvaluation(0), 1.0);
    ASSERT_EQ(cache.getEvaluation(1), 2.0);
    ASSERT_EQ(cache.getEvaluation(2), 7.8);
    ASSERT_EQ(cache.getEvaluation(3), 99.0);
    ASSERT_EQ(cache.getEvaluation(4), 0.0);
    ASSERT_EQ(cache.getEvaluation(5), 0.0);
    ASSERT_EQ(cache.getEvaluation(6), 0.0);
    ASSERT_EQ(cache.getEvaluation(7), 0.0);
    ASSERT_EQ(cache.getEvaluation(8), 56.0);

    ASSERT_EQ(cache.getIsDeadEnd(0), true);
    ASSERT_EQ(cache.getIsDeadEnd(1), false);
    ASSERT_EQ(cache.getIsDeadEnd(2), false);
    ASSERT_EQ(cache.getIsDeadEnd(3), false);
    ASSERT_EQ(cache.getIsDeadEnd(4), true);
    ASSERT_EQ(cache.getIsDeadEnd(5), false);
    ASSERT_EQ(cache.getIsDeadEnd(6), false);
    ASSERT_EQ(cache.getIsDeadEnd(7), false);
    ASSERT_EQ(cache.getIsDeadEnd(8), false);

    // Test that all defaults set when setIsDeadEnd is called for "much" larger node id
    cache.setIsDeadEnd(10, true);
    ASSERT_EQ(cache.getEvaluation(0), 1.0);
    ASSERT_EQ(cache.getEvaluation(1), 2.0);
    ASSERT_EQ(cache.getEvaluation(2), 7.8);
    ASSERT_EQ(cache.getEvaluation(3), 99.0);
    ASSERT_EQ(cache.getEvaluation(4), 0.0);
    ASSERT_EQ(cache.getEvaluation(5), 0.0);
    ASSERT_EQ(cache.getEvaluation(6), 0.0);
    ASSERT_EQ(cache.getEvaluation(7), 0.0);
    ASSERT_EQ(cache.getEvaluation(8), 56.0);
    ASSERT_EQ(cache.getEvaluation(9), 0.0);
    ASSERT_EQ(cache.getEvaluation(10), 0.0);

    ASSERT_EQ(cache.getIsDeadEnd(0), true);
    ASSERT_EQ(cache.getIsDeadEnd(1), false);
    ASSERT_EQ(cache.getIsDeadEnd(2), false);
    ASSERT_EQ(cache.getIsDeadEnd(3), false);
    ASSERT_EQ(cache.getIsDeadEnd(4), true);
    ASSERT_EQ(cache.getIsDeadEnd(5), false);
    ASSERT_EQ(cache.getIsDeadEnd(6), false);
    ASSERT_EQ(cache.getIsDeadEnd(7), false);
    ASSERT_EQ(cache.getIsDeadEnd(8), false);
    ASSERT_EQ(cache.getIsDeadEnd(9), false);
    ASSERT_EQ(cache.getIsDeadEnd(10), true);
}
