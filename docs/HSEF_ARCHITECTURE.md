# HSEF Architecture

On this page, we briefly describe the architecture to help you navigate the framework.
We encourage you to go through the tutorials before reading this content.

### Top-Level Directory

The top-level directory contains the following directory structure

- `.github/` includes code for running autotesting and other processes on Github
- `apps/` contains files that can be turned into binaries for running experiments. These can roughly be thought of as examples that can be customized to run experiments as you see fit
- `cmake/` has files useful for running CMake. This includes utility functions, how the static analyzers are run, which compiler warnings to use, and various settings for the build
- `core/` contains the main HSEF library. More detail on its organization will appear below in its own section
- `docs/` stores the files with additional documentation, including those for running Doxygen for generating the code documentation
- `tests/` contains unit tests for the code in `core/`. We note that this folder is organized identically to `core`, meaning that to find the file that tests a given file in `core`, one need only navigate the set directory structure in `tests`
- `tutorials` contains files that can be turned into binaries that help explain the core functionality of HSEF
- `visualizer` contains files related to an incomplete algorithm visualizer

### The Core Directory

The code for the main HSEF library can be found in `core/`. This includes defining the interfaces 
for the different main search components, code for the different environments, and code for the 
different test environments (ie. domains).

The place to start looking to understand the basic interfaces for HSEF is `core/search_basics`.
This folder contains abstract classes that define the basic interfacs for the main components of 
the HSEF framework. In particular, it defines the interfaces for transition system, goal testing,
search engines, node evaluation, and node containers. Defining standarding interfaces for these
components allows different ones to be plugged into as needed for different situations.

`core/environments` stores domain/environment specific things. For example, the `sliding_tile_puzzle`
directory contains files defining the states, actions, transition systems, and heuristics for the
sliding tile puzzle. Each different domain has such code in its own directory. Recall that the 
interfaces for each of these components is in `search_basics`

`core/engines/` has code for the different search algorithms. These algorithms inherit the interface
giben in `core/search_basics/search_engine.h`. Most will also inherit from `SingleStepSearchAlgorithm`
which is intended for single-core iterative search algorithms, in that it already includes code for
a large amount of required functionality for such algorithms. Notice that `engines/engine_components`
contains items like open lists and node containers that may be used by different engines.

`building_tools` has code for different search building blocks to be used when setting up an 
experiment. These include general evaluators (or evaluator properties), general goal tests, and
the basics of hash functions. The key property that groups these together is that they are general
and can be used across domains, unlike the code in the `environments` directory.

`environment_running` has utilities for running experiments. This includes objects for setting
search resource limits, running suites of experiments, and storing the results.

`logging` includes code for actually outputting the  results of experiments. A key part here is also 
logging the hyperparameters used to run an experiment. To facilitate this type of logging, most 
components are inherit from `SettingsLogger`, meaning there is a unified way to get all the relevant
settings of the component in question.

`utils` has a number of utilities that are useful during coding. These include utilities for handling
floating point comparison, reading and writing from files, and for combinatoric calculations.