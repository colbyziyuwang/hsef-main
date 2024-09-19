/**
 * Defines functions for graph file format parsing
 * 
 * @file graph_utils.h
 */

#ifndef GRAPH_UTILS_H_
#define GRAPH_UTILS_H_

#include "environments/graph/graph.h"

#include <sstream>
#include <string>

/**
 * Gets a graph object from a CSV file that represents an adjacency matrix for a graph.
 * The first line is a semi-colon separated list of all the labels of all the vertices of the graph
 * Each following line starts with the label of the starting vertex, followed by a semi-colon (;) separated list of
 * double values that represent the cost of the edge between the starting vertex and the corresponding vertex in the matrix
 * (listed in order from the first line). An empty entry means there is no edge between the vertices.
 * 
 * Example:
 * ;A;B;C;D;E
 * A;0;123;0;1;9001
 * B;1;0;0;0;0
 * C;5;0;1;0;0
 * D;;1;;1;
 * E;0;0;0;0;0
 * 
 * @param csv_ss std::stringstream object representing the adjacency matrix CSV
 * @return The graph corresponding to the adjacency matrix
 */
Graph getGraphFromCSVAdjacencyMatrix(std::stringstream& csv_ss);

/**
 * Parses a CSV file that represents an adjacency list for a graph.
 * Each line starts with the label of the starting vertex, followed by a semi-colon (;) separated list of
 * vertices that are connected to the starting vertex (adjacent to the starting vertex). The adjacent vertex
 * entry must include it's string label first, optionally followed by a space and a double value representing
 * the cost from going from the starting vertex to that adjacent vertex.
 * 
 * Examples:
 * a;b
 * b;c;d
 * 
 * a;b 17
 * b;c 99;d 6;z
 * 
 * @param csv_ss std::stringstream object representing the adjacency list CSV
 * @return The graph corresponding to the adjacency list
 */
Graph getGraphFromCSVAdjacencyList(std::stringstream& csv_ss);

/**
 * Returns the edge label between two vertices in a graph.
 * It generates a default label based on the from_vertex_label and to_vertex_label.
 *
 * @param from_vertex_label The label of the starting vertex
 * @param to_vertex_label The label of the adjacent vertex
 * @return The edge label between the two vertices
*/
std::string getEdgeLabel(const std::string& from_vertex_label, const std::string& to_vertex_label);

#endif /* GRAPH_UTILS_H_ */