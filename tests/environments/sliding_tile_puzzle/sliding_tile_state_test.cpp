#include <gtest/gtest.h>

#include "environments/sliding_tile_puzzle/sliding_tile_state.h"

/**
 * Tests SlidingTileState where we specify no additional values (ie. create a blank puzzle)
 * Member values should be 0. We also expect that two empty puzzles will be equal to each other.
 */
TEST(SlidingTileStateTests, createEmptyTilePuzzleTest) {
    SlidingTileState tile_puzzle_state_a;
    SlidingTileState tile_puzzle_state_b;

    ASSERT_EQ(tile_puzzle_state_a.m_permutation.size(), 0);

    ASSERT_EQ(tile_puzzle_state_a.m_num_rows, 0);
    ASSERT_EQ(tile_puzzle_state_a.m_num_cols, 0);
    ASSERT_EQ(tile_puzzle_state_a.m_blank_loc, 0);

    ASSERT_TRUE(tile_puzzle_state_a == tile_puzzle_state_b);
    ASSERT_FALSE(tile_puzzle_state_a != tile_puzzle_state_b);
}

/**
 * Tests SlidingTileState where we specify the number of rows and columns.
 * We expect that two puzzles with the same parameters to be equal to each other.
 */
TEST(SlidingTileStateTests, createSameTilePuzzleTest) {
    SlidingTileState tile_puzzle_state_a(363, 38);
    SlidingTileState tile_puzzle_state_b(363, 38);

    ASSERT_EQ(tile_puzzle_state_a.m_permutation.size(), 363 * 38);

    ASSERT_EQ(tile_puzzle_state_a.m_num_rows, 363);
    ASSERT_EQ(tile_puzzle_state_a.m_num_cols, 38);
    ASSERT_EQ(tile_puzzle_state_a.m_blank_loc, 0);

    // Checks that it is creating standard permutation [0, 1, 2, ..., num_cols * num_rows - 1]
    for (Tile tile_num = 0;
              tile_num <= tile_puzzle_state_a.m_num_cols * tile_puzzle_state_a.m_num_rows - 1;
              tile_num++) {
        ASSERT_EQ(tile_puzzle_state_a.m_permutation[tile_num], tile_num);
        ASSERT_EQ(tile_puzzle_state_b.m_permutation[tile_num], tile_num);
    }

    ASSERT_TRUE(tile_puzzle_state_a == tile_puzzle_state_b);
    ASSERT_FALSE(tile_puzzle_state_a != tile_puzzle_state_b);
}

/**
 * Tests SlidingTileState where we specify a permutation of states and number of rows and columns.
 * We expect that two puzzles with the same parameters to be equal to each other.
 */
TEST(SlidingTileStateTests, createSameTilePuzzleWithPermsTest) {
    std::vector<Tile> perm{1, 2, 3, 4, 5, 0, 7, 8, 9, 10, 11, 6, 12, 13, 14, 15};

    SlidingTileState tile_puzzle_state_a(perm, 4, 4);
    SlidingTileState tile_puzzle_state_b(perm, 4, 4);

    ASSERT_EQ(tile_puzzle_state_a.m_permutation.size(), 4 * 4);

    ASSERT_EQ(tile_puzzle_state_a.m_num_rows, 4);
    ASSERT_EQ(tile_puzzle_state_a.m_num_cols, 4);
    ASSERT_EQ(tile_puzzle_state_a.m_blank_loc, 5);

    ASSERT_TRUE(tile_puzzle_state_a == tile_puzzle_state_b);
    ASSERT_FALSE(tile_puzzle_state_a != tile_puzzle_state_b);
}

/**
 * Tests SlidingTileState where we specify a permutation of states and number of rows and columns.
 * We expect that two puzzles with different row/col parameters but with same state permutation
 * to be the same state space.
 */
TEST(SlidingTileStateTests, createDifferentTilePuzzleWithSamePermsTest) {
    std::vector<Tile> perm{1, 2, 3, 4, 5, 0, 7, 8, 9, 10, 11, 6, 12, 13, 14, 15};

    SlidingTileState tile_puzzle_state_a(perm, 4, 4);
    SlidingTileState tile_puzzle_state_b(perm, 2, 8);

    ASSERT_EQ(tile_puzzle_state_a.m_permutation.size(), 4 * 4);
    ASSERT_EQ(tile_puzzle_state_a.m_permutation.size(), 2 * 8);

    ASSERT_EQ(tile_puzzle_state_a.m_num_rows, 4);
    ASSERT_EQ(tile_puzzle_state_a.m_num_cols, 4);
    ASSERT_EQ(tile_puzzle_state_a.m_blank_loc, 5);

    ASSERT_EQ(tile_puzzle_state_b.m_num_rows, 2);
    ASSERT_EQ(tile_puzzle_state_b.m_num_cols, 8);
    ASSERT_EQ(tile_puzzle_state_b.m_blank_loc, 5);

    ASSERT_TRUE(tile_puzzle_state_a == tile_puzzle_state_b);
    ASSERT_FALSE(tile_puzzle_state_a != tile_puzzle_state_b);
}

/**
 * Tests SlidingTileState where we specify the number of rows and columns.
 * We expect that two puzzles with the different parameters to be unequal to each other.
 */
TEST(SlidingTileStateTests, createDifferentTilePuzzleTest) {
    SlidingTileState tile_puzzle_state_a(363, 38);
    SlidingTileState tile_puzzle_state_b(496, 114);


    ASSERT_EQ(tile_puzzle_state_a.m_permutation.size(), 363 * 38);
    ASSERT_EQ(tile_puzzle_state_b.m_permutation.size(), 496 * 114);

    ASSERT_EQ(tile_puzzle_state_a.m_num_rows, 363);
    ASSERT_EQ(tile_puzzle_state_a.m_num_cols, 38);
    ASSERT_EQ(tile_puzzle_state_a.m_blank_loc, 0);

    ASSERT_EQ(tile_puzzle_state_b.m_num_rows, 496);
    ASSERT_EQ(tile_puzzle_state_b.m_num_cols, 114);
    ASSERT_EQ(tile_puzzle_state_b.m_blank_loc, 0);

    ASSERT_FALSE(tile_puzzle_state_a == tile_puzzle_state_b);
    ASSERT_TRUE(tile_puzzle_state_a != tile_puzzle_state_b);
}

/**
 * Tests SlidingTileState where we specify a permutation of states and number of rows and columns.
 * We expect that two puzzles with different row/col parameters and state permutation
 * to be a different state space.
 */
TEST(SlidingTileStateTests, createDifferentTilePuzzleWithDifferentPermsTest) {
    std::vector<Tile> perm_1{1, 2, 3, 4, 5, 0, 7, 8, 9, 10, 11, 6, 12, 13, 14, 15};
    std::vector<Tile> perm_2{1, 2, 3, 4, 5, 8, 7, 0, 6};

    SlidingTileState tile_puzzle_state_a(perm_1, 4, 4);
    SlidingTileState tile_puzzle_state_b(perm_2, 3, 3);

    ASSERT_EQ(tile_puzzle_state_a.m_permutation.size(), 4 * 4);
    ASSERT_EQ(tile_puzzle_state_b.m_permutation.size(), 3 * 3);

    ASSERT_EQ(tile_puzzle_state_a.m_num_rows, 4);
    ASSERT_EQ(tile_puzzle_state_a.m_num_cols, 4);
    ASSERT_EQ(tile_puzzle_state_a.m_blank_loc, 5);

    ASSERT_EQ(tile_puzzle_state_b.m_num_rows, 3);
    ASSERT_EQ(tile_puzzle_state_b.m_num_cols, 3);
    ASSERT_EQ(tile_puzzle_state_b.m_blank_loc, 7);

    ASSERT_FALSE(tile_puzzle_state_a == tile_puzzle_state_b);
    ASSERT_TRUE(tile_puzzle_state_a != tile_puzzle_state_b);
}
