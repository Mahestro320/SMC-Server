#include <sstream>
#include <vector>
#include "util/string.hpp"

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

std::string util::string::getStringFromArray(const char* array, uint64_t size) {
    uint64_t end_offset{size};
    for (uint64_t i{0}; i < size; ++i) {
        if (array[i] == 0x00) {
            end_offset = i;
            break;
        }
    }
    return std::string{array, end_offset};
}
