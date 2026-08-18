#include <string>
#include <vector>
#include <fstream>
class FileIo {
public:
    FileIo() = default;
    ~FileIo() = default;
    static std::vector<std::string> read_lines(const std::string& filePath);
};
