#pragma once

#include <filesystem>

namespace dirs {

constexpr std::string_view COMPILATION_CURR_FILE_STR{__FILE__};
const auto COMPILATION_CURR_FILE{std::filesystem::path(COMPILATION_CURR_FILE_STR)};
const auto COMPILATION_ROOT{COMPILATION_CURR_FILE.parent_path().parent_path().parent_path()};

const auto ROOT{std::filesystem::current_path()};
const auto LOGS{ROOT / "logs"};

// storage directory
const auto STRG{ROOT / "srvdir"};
const auto S_USERS{STRG / "users"};
const auto S_NETWORK{STRG / "network"};
const auto S_NETWORK_SHARED{S_NETWORK / "shared"};

}; // namespace dirs
