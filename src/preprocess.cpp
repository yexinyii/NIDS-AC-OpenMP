#include "preprocess.h"
#include <cctype>

// HTTP预处理：统一转为小写，避免大小写导致漏检

std::string to_lowercase(const std::string& text) {
    std::string result = text;

    for (char& c : result) {
        c = std::tolower(static_cast<unsigned char>(c));
    }

    return result;
}
