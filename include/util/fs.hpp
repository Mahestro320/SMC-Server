#pragma once

#include "system/beg.hpp"

#include <filesystem>

namespace util::fs {

extern bool haveDriveLetter(const std::filesystem::path& path);

} // namespace util::fs
