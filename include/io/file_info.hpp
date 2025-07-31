#pragma once

#include <string>
#include "file_type.hpp"

struct FileInfo {
    std::string name{};
    FileType type{};
    size_t size{};
};
