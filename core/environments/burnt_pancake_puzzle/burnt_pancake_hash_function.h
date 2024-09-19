#ifndef BURNT_PANCAKE_HASH_FUNCTION_H_
#define BURNT_PANCAKE_HASH_FUNCTION_H_

#include "building_tools/hashing/permutation_hash_function.h"
#include "burnt_pancake_state.h"

using BurntPancakeHashFunction = PermutationHashFunction<BurntPancakeState>;

#endif  //BURNT_PANCAKE_HASH_FUNCTION_H_
