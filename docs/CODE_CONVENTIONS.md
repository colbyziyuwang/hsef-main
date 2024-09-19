# Code Conventions

In this file, we describe the code naming coventions used in HSEF. These are largely enforced
by clang-format and clang-tidy.

### File Naming

Header files have extension `.h`, while source files have extension `.cpp`.
All words are lower case, separated by an underscore.
The file format is enforced  by the `.clang-format` file.

### Variable, Method, and Class Naming

All such naming conventions can be found in the `clang-tidy` file. In brief, classes and methods use
camelCase, where the first letter of classes are upper case while the first letter of methods are
lower case (ie. `SearchMethod` vs `getLastSolutionPlan`). Variables use snake case, where all words
are in lower case letter separated by underscores (ie. `transition_function`). Member variables follow
the same convention but always start with a `m_` (ie. `m_open_list`). This is intended to make it easy 
to differentiate between member variables and local variables..

### Test Conventions

TODO

### CMake Naming Conventions

TODO