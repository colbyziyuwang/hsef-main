#ifndef SLIDING_TILE_HASH_FUNCTION_H_
#define SLIDING_TILE_HASH_FUNCTION_H_

#include "building_tools/hashing/permutation_hash_function.h"
#include "sliding_tile_state.h"

using SlidingTileHashFunction = PermutationHashFunction<SlidingTileState>;

#endif  //SLIDING_TILE_HASH_FUNCTION_H_
