#pragma once

#include <string>

namespace util::string {

extern std::string formatDigits(int number, uint16_t digits);
extern std::string ptrToString(const void* ptr);
extern std::string getStringFromArray(const char* array, uint64_t size);

} // namespace util::string
