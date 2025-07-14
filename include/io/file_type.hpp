#pragma once

#include <string>

enum class FileType : uint8_t {
    None,
    File,
    Directory,
};

namespace file_type {

extern std::string getName(FileType id);

}
