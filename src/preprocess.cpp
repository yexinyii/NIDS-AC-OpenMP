#include "preprocess.h"
#include <cctype>

std::string to_lowercase(const std::string& text)
{
    std::string result = text;

    for (char& c : result)
    {
        c = std::tolower(static_cast<unsigned char>(c));
    }

    return result;
}
