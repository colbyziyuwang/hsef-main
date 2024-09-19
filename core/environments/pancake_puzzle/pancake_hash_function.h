#ifndef PANCAKE_HASH_FUNCTION_H_
#define PANCAKE_HASH_FUNCTION_H_

#include "building_tools/hashing/permutation_hash_function.h"
#include "pancake_state.h"

using PancakeHashFunction = PermutationHashFunction<PancakeState>;

#endif  //PANCAKE_HASH_FUNCTION_H_
