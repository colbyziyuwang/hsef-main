#include <gtest/gtest.h>

#include "utils/string_utils.h"

#include <string>
#include <vector>

/**
 * Checks that bool to string works as expected
 */
TEST(StringUtilsTests, boolToStringTest) {
    EXPECT_EQ(boolToString(true), "true");
    EXPECT_EQ(boolToString(false), "false");
}

/**
 * Checks that split works as expected
 */
TEST(StringUtilsTests, splitTest) {
    std::vector<std::string> expected_tokens{"the", "quick", "brown", "fox", "jumps", "over", "the", "lazy", "dog."};

    std::vector<std::string> tokens = split("the quick brown fox jumps over the lazy dog.", ' ');

    ASSERT_EQ(tokens.size(), expected_tokens.size()) << "Vectors tokens and expected_tokens are of unequal length";

    for (unsigned i = 0; i < tokens.size(); ++i) {
        EXPECT_EQ(tokens[i], expected_tokens[i]) << "Vectors tokens and expected_tokens differ at index " << i;
    }
}

/**
 * Checks that vector to string works as expected
 */
TEST(StringUtilsTests, vecToStringTest) {
    std::vector<std::string> strings{"the", "quick", "brown", "fox", "jumps", "over", "the", "lazy", "dog."};
    EXPECT_EQ(vectorToString(strings), "[the quick brown fox jumps over the lazy dog.]");
}

/** 
 * Checks that roundAndToString method works as expected
 */
TEST(StringUtilsTests, roundAndToStringTest) {
    ASSERT_EQ(roundAndToString(123.86695443, 0), "124");
    ASSERT_EQ(roundAndToString(123.86695443, 1), "123.9");
    ASSERT_EQ(roundAndToString(123.86695443, 2), "123.87");
    ASSERT_EQ(roundAndToString(123.86695443, 3), "123.867");
    ASSERT_EQ(roundAndToString(123.86695443, 4), "123.867");
    ASSERT_EQ(roundAndToString(123.86695443, 5), "123.86695");
    ASSERT_EQ(roundAndToString(123.86695443, 6), "123.866954");
    ASSERT_EQ(roundAndToString(123.86695443, 7), "");
    ASSERT_EQ(roundAndToString(123.866, 6), "123.866");

    ASSERT_EQ(roundAndToString(100, 5), "100.0");
    ASSERT_EQ(roundAndToString(2, 0), "2");
    ASSERT_EQ(roundAndToString(-100, 2), "-100.0");
    ASSERT_EQ(roundAndToString(0, 2), "0.0");
    ASSERT_EQ(roundAndToString(0, 0), "0");
}

/**
* Tests that findCharOccurance works as expected
*/
TEST(StringUtilsTests, findCharOccuranceTest) {
    ASSERT_EQ(findCharOccurance("tesT Strings", 't', 1), 0);
    ASSERT_EQ(findCharOccurance("tesT Strings", 't', 2), 6);
    ASSERT_EQ(findCharOccurance("tesT Strings", 't', 3), -1);
    ASSERT_EQ(findCharOccurance("tesT Strings", 'T', 1), 3);
    ASSERT_EQ(findCharOccurance("tesT Strings", 'T', -1), -1);
    ASSERT_EQ(findCharOccurance("", 'T', 1), -1);

    ASSERT_DEBUG_DEATH(findCharOccurance("", 'T', 0), "");
}