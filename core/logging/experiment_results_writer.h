#ifndef EXPERIMENT_RESULTS_WRITER_H_
#define EXPERIMENT_RESULTS_WRITER_H_

#include "experiment_running/experiment_results.h"
#include "logging/yaml_logging.h"
#include "utils/string_utils.h"

#include <optional>
#include <sstream>
#include <string>
#include <vector>

/**
 * Gets the vector of results in a CSV format. This includes a header
 *
 * @tparam Action_t The type of action
 * @param results The results of a set of experiments
 * @param delim The delimiter to use
 * @return A string representation of the experiments in CSV format.
 */
template<class Action_t>
std::string getResultsVectorAsCSV(const std::vector<ExperimentResults<Action_t>>& results, const std::string& delim = ",");

/**
 * Gets the experiment result as a delimiter separated string. No header is included.
 *
 * Largely to be used as a helper for generating CSV style files when given a vector of experiment result objects.
 *
 * @tparam Action_t The type of action
 * @param result The result of an experiment
 * @param experiment_name The name of the experiment. Skips if it is null
 * @param delim The delimiter to use
 * @return A string representation of the experiments in CSV format
 */
template<class Action_t>
std::string getResultAsCSV(const ExperimentResults<Action_t>& result, const std::optional<std::string>& experiment_name = std::nullopt, const std::string& delim = ",");

/**
 * Gets the header for a CSV file that would contain results of the given form.
 *
 * @tparam Action_t The type of action
 * @param result The result of an experiment
 * @param experiment_name The name of the experiment. Skips if it is null
 * @param delim The delimiter to use
 * @return A string representation of the header
 */
template<class Action_t>
std::string getCSVHeader(const ExperimentResults<Action_t>& result, const std::string& delim = ",");


/**
 * Returns the experiment result data in YAML format. Allows for a specific prefix to be added at the front
 * of each line. This is useful when the output is to be used nested inside a larger YAML file.
 *
 * @tparam Action_t The action type
 * @param results The result of an experiment
 * @param prefix A prefix to add at the beginning of every line
 * @return A string representation of the experiment result in YAML format
 */
template<class Action_t>
std::string getResultAsYAML(const ExperimentResults<Action_t>& result, std::string prefix = "");

/**
 * Returns a string representation of a vector of experiment results in YAML format.
 *
 * @tparam Action_t The action type
 * @param results The results of a set of experiments
 * @param experiment_id The name of the experiment
 * @return A string representation of the experiments in YAML format
 */
template<class Action_t>
std::string getResultsVectorAsYAML(const std::vector<ExperimentResults<Action_t>>& results);

template<class Action_t>
std::string getResultAsCSV(const ExperimentResults<Action_t>& result, const std::optional<std::string>& experiment_id, const std::string& delim) {
    std::stringstream output("");

    if (experiment_id.has_value()) {
        output << experiment_id.value() << delim;
    }

    output << boolToString(result.m_has_found_plan) << delim << roundAndToString(result.m_plan_cost, 6);
    output << delim << result.m_plan.size();

    for (const auto& [stat_name, stat_value] : result.m_standard_stats.getStatsLog()) {
        output << delim << stat_value;
    }
    for (const auto& [stat_name, stat_value] : result.m_engine_specific_stats) {
        output << delim << stat_value;
    }
    return output.str();
}

template<class Action_t>
std::string getCSVHeader(const ExperimentResults<Action_t>& result, const std::string& delim) {
    std::stringstream output("");

    output << "run_id" << delim << "plan_found" << delim << "plan_cost" << delim << "plan_length";

    for (const auto& [stat_name, stat_value] : result.m_standard_stats.getStatsLog()) {
        output << delim << stat_name;
    }
    for (const auto& [stat_name, stat_value] : result.m_engine_specific_stats) {
        output << delim << stat_name;
    }
    return output.str();
}

template<class Action_t>
std::string getResultsVectorAsCSV(const std::vector<ExperimentResults<Action_t>>& results, const std::string& delim) {
    std::stringstream output("");

    output << getCSVHeader(results[0]);

    unsigned experiment_num = 1;
    for (const auto& result : results) {
        output << "\n"
               << getResultAsCSV(result, std::to_string(experiment_num), delim);
        experiment_num++;
    }
    return output.str();
}

template<class Action_t>
std::string getResultAsYAML(const ExperimentResults<Action_t>& result, std::string prefix) {
    //TODO: modify what is added based on writing flags
    using namespace yamlHelperFunctions;

    char line_end = '\n';
    std::stringstream output;

    output << prefix << dictionaryStr("transition_system") << line_end;
    startMembers(prefix);
    output << getYAMLString(result.m_transitions_settings, prefix);
    endMembers(prefix);

    output << prefix << dictionaryStr("goal_test") << line_end;
    startMembers(prefix);
    output << getYAMLString(result.m_goal_test_settings, prefix);
    endMembers(prefix);

    output << prefix << dictionaryStr("search_engine") << line_end;
    startMembers(prefix);
    output << getYAMLString(result.m_engine_settings, prefix);
    endMembers(prefix);

    output << prefix << dictionaryStr("results") << line_end;

    startMembers(prefix);
    output << prefix << dictionaryStr("plan_found", boolToString(result.m_has_found_plan)) << line_end;
    output << prefix << dictionaryStr("plan_cost", roundAndToString(result.m_plan_cost, 6)) << line_end;
    output << prefix << dictionaryStr("plan", vectorToYAMLString(result.m_plan)) << line_end;
    output << prefix << dictionaryStr("stats") << line_end;

    startMembers(prefix);

    for (const auto& [stat_name, stat_value] : result.m_standard_stats.getStatsLog()) {
        output << prefix << listEntry(dictionaryStr(stat_name, stat_value)) << line_end;
    }
    for (const auto& [stat_name, stat_value] : result.m_engine_specific_stats) {
        output << prefix << listEntry(dictionaryStr(stat_name, stat_value)) << line_end;
    }

    endMembers(prefix);
    endMembers(prefix);

    return output.str();
}

template<class Action_t>
std::string getResultsVectorAsYAML(const std::vector<ExperimentResults<Action_t>>& results) {
    using namespace yamlHelperFunctions;

    std::stringstream output;
    std::string prefix;

    unsigned experiment_num = 1;
    for (auto& res : results) {
        output << listEntry(dictionaryStr("experiment_" + std::to_string(experiment_num++))) << '\n';
        startMembers(prefix);
        output << getResultAsYAML(res, prefix);
        endMembers(prefix);
        output << '\n';
    }

    return output.str();
}
#endif  // !OUTPUT_WRITER_H_
