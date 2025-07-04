#pragma once

#include <string>
#include "util/version.hpp"

namespace common {

inline std::string main_thread_id{};
inline bool is_running{true};

inline constexpr Version VERSION{0, 1, 0};

} // namespace common
