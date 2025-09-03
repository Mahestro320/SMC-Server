#include "util/string.hpp"

#include <sstream>
#include <vector>

void util::string::toLowercase(std::string& string) {
    for (char& c : string) {
        c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    }
}

std::string util::string::formatDigits(int number, uint16_t digits) {
    std::vector<char> buffer(digits + 1);
    snprintf(buffer.data(), buffer.size(), "%0*d", digits, number);
    return std::string(buffer.data());
}

std::string util::string::ptrToString(const void* ptr) {
    std::ostringstream oss;
    oss << ptr;
    return "0x" + oss.str();
}

std::string util::string::getStringFromArray(const char* array, size_t size) {
    size_t end_offset{size};
    for (size_t i{0}; i < size; ++i) {
        if (array[i] == 0x00) {
            end_offset = i;
            break;
        }
    }
    return std::string{array, end_offset};
}
