#include "grid_pathfinding_scenario_running.h"
#include "experiment_running/experiment_results.h"
#include "experiment_running/experiment_runner.h"
#include "experiment_running/search_resource_limits.h"
#include "grid_location.h"
#include "grid_map.h"
#include "grid_pathfinding_action.h"
#include "grid_pathfinding_transitions.h"
#include "logging/experiment_results_writer.h"
#include "search_basics/search_engine.h"
#include "utils/evaluator_utils.h"
#include "utils/io_utils.h"
#include "utils/string_utils.h"

#include <cassert>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

std::vector<GridPathfindingScenario> loadScenarioFile(
          const std::string& scenario_file, const std::string& maps_directory) {
    char delim = '\t';

    std::vector<GridPathfindingScenario> scenarios;

    std::string file_str = loadFileIntoStringSteam(scenario_file).str();
    file_str.erase(0, file_str.find('\n') + 1);
    std::stringstream file_content(file_str);
    std::string line;

    while (getline(file_content, line)) {
        GridPathfindingScenario scenario_data;

        scenario_data.m_map_path = maps_directory + split(line, delim)[1];

        int start_index = findCharOccurance(line, delim, 4) + 1;
        assert(start_index > 0);

        std::vector<std::string> data = split(line.substr(static_cast<std::size_t>(start_index)), delim);

        assert(data.size() == 5);
        scenario_data.m_start_state = GridLocation(stoi(data[0]), stoi(data[1]));
        scenario_data.m_goal_state = GridLocation(stoi(data[2]), stoi(data[3]));
        scenario_data.m_octile_optimal_cost = stod(data[4]);

        scenarios.emplace_back(scenario_data);
    }

    return scenarios;
}

std::vector<ExperimentResults<GridDirection>> runScenarioExperiments(SearchEngine<GridLocation, GridDirection>& engine,
          const SearchResourceLimits& resource_limits, const std::vector<GridPathfindingScenario>& scenarios, bool incremental_output) {
    std::vector<ExperimentResults<GridDirection>> results;
    results.reserve(scenarios.size());

    if (scenarios.empty()) {
        return results;
    }

    std::string current_map_path = scenarios[0].m_map_path;
    std::string map_str = loadFileIntoStringSteam(current_map_path).str();
    std::stringstream map_info(map_str.substr(map_str.find('\n') + 1));  //removes 'type octile' TODO: Handle properly

    GridMap map(map_info);
    GridPathfindingTransitions transitions(&map);
    transitions.setConnectionType(GridConnectionType::eight);

    auto base_evals = engine.getBaseEvaluators();
    auto all_evaluators = getAllEvaluators(base_evals);

    engine.setResourceLimits(resource_limits);
    engine.setTransitionSystem(transitions);

    unsigned experiment_num = 1;
    for (const auto& scenario : scenarios) {
        if (current_map_path != scenario.m_map_path) {
            map_str = loadFileIntoStringSteam(scenario.m_map_path).str();
            map_info = std::stringstream(map_str.substr(map_str.find('\n') + 1));
            map = GridMap(map_info);
            current_map_path = scenario.m_map_path;
        }
        results.emplace_back(runExperiment(engine, scenario.m_start_state, scenario.m_goal_state, all_evaluators));
        if (incremental_output) {
            if (experiment_num == 1) {
                std::cout << getCSVHeader(results[0]) << "\n";
            }
            std::cout << getResultAsCSV(results.back(), std::to_string(experiment_num)) << "\n";
        }
        experiment_num++;
    }

    return results;
}
