#pragma once

#include "system/beg.hpp"

#include <string>
#include "file_type.hpp"

struct FileInfo {
    std::string name{};
    FileType type{};
    size_t size{};
};
