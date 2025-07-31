#pragma once

#include <string>

namespace util::string {

extern std::string formatDigits(int number, uint16_t digits);
extern std::string ptrToString(const void* ptr);
extern std::string getStringFromArray(const char* array, size_t size);

} // namespace util::string
