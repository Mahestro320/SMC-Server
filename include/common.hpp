#pragma once

#include "system/beg.hpp"

#include <string>
#include "util/version.hpp"

namespace common {

inline constexpr char UNDEFINED_TEXT[]{"undefined"};
inline constexpr Version VERSION{0, 4, 0};

inline std::string main_thread_id{};

} // namespace common
