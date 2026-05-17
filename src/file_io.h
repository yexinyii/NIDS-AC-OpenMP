#include <string>
#include <vector>
#include <fstream>
class FileIO {
public:
    FileIO() = default;
    ~FileIO() = default;
    static std::vector<std::string> readLines(const std::string& filePath);
};