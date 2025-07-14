#pragma once

#include <string>

enum class ResponseId : uint8_t {
    Ok = 0x00,
    UnknownRequest = 0x01,
    UnknownUsername = 0x02,
    InvalidPassword = 0x40,
    InvalidData = 0x41,
    InvalidBufferSize = 0x42,
    NotLogged = 0x80,
    IOFileNotExists = 0xC0,
    IOFileNotReadable = 0xC1,
    IONotDirectory = 0xC2,
    IONotFile = 0xC3,
    IOInvalidPath = 0xC4,
    IONonAbsolutePath = 0xC5,
};

namespace network::response {

extern std::string getName(ResponseId id);

}
