/**
 * Defines functions for file input/output.
 * 
 * @file io_utils.h
 */

#ifndef IO_UTILS_H_
#define IO_UTILS_H_

#include <sstream>
#include <string>

/**
 * Loads a file with the given file name and returns a std::stringstream object representing 
 * the contents of that file.
 *
 * @param file_name The name of the file to read.
 * @return std::stringstream object representing the contents of that file.
 */
std::stringstream loadFileIntoStringSteam(const std::string& file_name);

/**
 * Writes the string data to a file. Will overwrite the file if it already exists.
 *
 * @param data the string to be added to the file
 * @param output_file The directory and name of the output file
 */
void writeStringToFile(const std::string& data, const std::string& output_file);

#endif /* IO_UTILS_H_ */