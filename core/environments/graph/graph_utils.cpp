#include "graph_utils.h"
#include "graph.h"
#include "utils/string_utils.h"

#include <cassert>
#include <cstdlib>
#include <sstream>
#include <string>
#include <vector>

Graph getGraphFromCSVAdjacencyMatrix(std::stringstream& csv_ss) {
    Graph graph;

    std::string line;
    std::vector<std::string> tokens;
    std::vector<std::string> vertices;

    // Handles top line of matrix which contains the list of vertices
    getline(csv_ss, line);
    tokens = split(line, ';');
    for (const std::string& token : tokens) {
        if (!token.empty()) {
            graph.addVertex(token);
            vertices.push_back(token);
        }
    }
    // Handles each line of the adjacency matrix
    unsigned vertex_count = 0;
    while (getline(csv_ss, line)) {
        tokens = split(line, ';');
        if (tokens.empty()) {
            break;
        }
        std::string from_vertex = tokens[0];
        for (unsigned to_vertex = 1; to_vertex < tokens.size(); to_vertex++) {
            if (!tokens[to_vertex].empty()) {
                double cost = std::stod(tokens[to_vertex]);
                auto [add_succeeded, vertex_id] = graph.addEdge(from_vertex, vertices[to_vertex - 1], cost);
                assert(add_succeeded);
            }
        }
        vertex_count++;
    }
    assert(vertex_count == vertices.size());
    return graph;
}

Graph getGraphFromCSVAdjacencyList(std::stringstream& csv_ss) {
    Graph graph;
    std::size_t found = 0;
    std::string line;
    std::vector<std::string> tokens;

    while (getline(csv_ss, line)) {
        tokens = split(line, ';');
        if (tokens.empty()) {
            break;
        }
        std::string from_vertex = tokens[0];
        for (unsigned to_vertex = 1; to_vertex < tokens.size(); to_vertex++) {
            double cost = 1.0;
            found = tokens[to_vertex].find(' ');
            if (found != std::string::npos) {
                cost = std::stod(tokens[to_vertex].substr(found));
            }

            auto [add_succeeded, vertex_id] = graph.addEdge(from_vertex, tokens[to_vertex].substr(0, found), cost);
            assert(add_succeeded);
        }
    }
    return graph;
}

std::string getEdgeLabel(const std::string& from_vertex_label, const std::string& to_vertex_label) {
    return from_vertex_label + "->" + to_vertex_label;
}