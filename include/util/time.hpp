#pragma once

#include "system/beg.hpp"

#include <string>

namespace util::time {

extern uint64_t getSeconds();
extern uint64_t getMillis();
extern uint64_t getNanos();

extern std::string formatTime(uint64_t timestamp, const std::string& format = "");

} // namespace util::time
