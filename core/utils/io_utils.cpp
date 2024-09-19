#include "io_utils.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

std::stringstream loadFileIntoStringSteam(const std::string& file_name) {
    std::ifstream input_file(file_name);
    std::stringstream buffer;
    if (input_file.fail()) {
        std::cerr << "Could not read in map file. ";
        std::cerr << "Map reading failed.\n";
        return buffer;
    }
    buffer << input_file.rdbuf();
    return buffer;
}

void writeStringToFile(const std::string& data, const std::string& output_file) {
    //TODO: Add tests
    std::filesystem::path directory = std::filesystem::path(output_file).parent_path();
    if (!directory.string().empty() && !std::filesystem::is_directory(directory)) {
        if (!std::filesystem::create_directories(directory)) {
            std::cerr << "Failed to create directory: " << directory << "\n";
            return;
        }
    }

    std::ofstream file;
    file.open(output_file, std::ofstream::trunc);
    if (!file.is_open()) {
        std::cerr << "Failed to write to file: " << output_file << "\n";
        return;
    }
    file << data << "\n";
    file.close();
}