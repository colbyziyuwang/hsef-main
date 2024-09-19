set(ID_FILES # cmake-format: sortable
             id_engine.h id_engine_params.cpp id_engine_params.h)

list(TRANSFORM ID_FILES PREPEND engines/iterative_deepening/)

set(ID_FILES
    ${ID_FILES}
    PARENT_SCOPE)

message("In iterative_deepening ${ID_FILES}")
