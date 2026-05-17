#include "file_io.h"
#include <iostream>
int main() {
    std::string filePath = "dataset/http_all.txt";
    try {
        std::vector<std::string> data_lines = FileIO::readLines(filePath);
        for (const auto& line : data_lines) {
            std::cout << line << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << " " << e.what() << std::endl;
    }
    filePath = "rules/rules.txt";
    try {
        std::vector<std::string> rule_lines = FileIO::readLines(filePath);
        for (const auto& line : rule_lines) {
            std::cout << line << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    return 0;
}