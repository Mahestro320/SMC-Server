#pragma once

#include "system/beg.hpp"

#include <string>

enum class ResponseId : uint8_t {
    Ok = 0x00,
    UnknownRequest,
    UnknownUsername,
    InvalidPassword = 0x40,
    InvalidData,
    InvalidBufferSize,
    InvalidPermissions,
    NotLogged = 0x80,
    IOError = 0xC0,
    IOFileNotExists,
    IOFileNotReadable,
    IONotDirectory,
    IONotFile,
    IOInvalidPath,
    IONonAbsolutePath,
    IOAlreadyExists,
};

namespace network::response {

extern std::string getName(ResponseId id);

}
