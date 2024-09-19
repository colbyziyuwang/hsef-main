set(HASHING_BUILDING_FILES
    # cmake-format: sortable
    permutation_hash_function.h signed_permutation_hash_function.h state_hash_function.h state_string_hash_function.h)

list(TRANSFORM HASHING_BUILDING_FILES PREPEND building_tools/hashing/)
set(HASHING_BUILDING_FILES
    ${HASHING_BUILDING_FILES}
    PARENT_SCOPE)
