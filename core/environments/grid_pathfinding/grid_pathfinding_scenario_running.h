#ifndef GRID_PATHFINDING_SCENARIO_RUNNING_H_
#define GRID_PATHFINDING_SCENARIO_RUNNING_H_

#include "experiment_running/experiment_results.h"
#include "experiment_running/search_resource_limits.h"
#include "grid_location.h"
#include "grid_pathfinding_action.h"
#include "search_basics/search_engine.h"

#include <string>
#include <vector>

/**
 * Contains utilities for loading scenario files from Nathan Sturtevant's pathfinding benchmark,
 * and running experiments with them.
 */

/**
* Struct to contain the needed data to run an experiment from a Nathan Sturtevant's pathfinding benchmark scenario
*/
struct GridPathfindingScenario {
    std::string m_map_path;  ///< The path to the map file
    GridLocation m_start_state;  ///< The start state for this experiment
    GridLocation m_goal_state;  ///< The goal state for this experiment
    double m_octile_optimal_cost = 0.0;  ///< The known optimal cost for this experiment
};

/**
  * Extracts the needed data from a scenario file and returns a vector of scenarios.
  *
  * Note: the subdirectories of the map files must follow the same names as the data in the scenario file
  *
  * @param scenario_file The name and path to the scenario file
  * @param maps_directory The path to the folder containing the map files for the scenarios
  * @return The extracted scenarios
  */
std::vector<GridPathfindingScenario> loadScenarioFile(
          const std::string& scenario_file, const std::string& maps_directory);

/**
 * Runs the experiments given as a list of scenarios.
 *
 * @param engine The engine to use for the experiments
 * @param resource_limits The resource limits to place on the experiments
 * @param scenarios The list of scenarios
 * @param incremental_output Whether to output results as a CSV incrementally
 * @return The result of the experiments
 */
std::vector<ExperimentResults<GridDirection>> runScenarioExperiments(SearchEngine<GridLocation, GridDirection>& engine,
          const SearchResourceLimits& resource_limits, const std::vector<GridPathfindingScenario>& scenarios, bool incremental_output = false);

#endif  //GRID_PATHFINDING_SCENARIO_RUNNING_H_