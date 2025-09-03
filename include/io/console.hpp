#pragma once

#include "system/beg.hpp"

#include <string>

namespace console {

inline constexpr size_t QUEUE_MAX_SIZE{0xFF};

extern void init();

namespace out {

extern void inf(const std::string& message = "");
extern void warn(const std::string& message = "");
extern void err(const std::string& message = "");
extern void err(const std::exception& e);

} // namespace out

} // namespace console
