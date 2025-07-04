#pragma once

#include <string>

enum class ResponseId : uint8_t {
    Ok = 0x00,
    UnknownRequest = 0x01,
    UnknownUsername = 0x02,
    InvalidPassword = 0x40,
    InvalidData = 0x41,
    NotLogged = 0x80,
    IOFileNotExists = 0xC0,
    IONotDirectory = 0xC1,
    IOInvalidPath = 0xC2,
    IONonAbsolutePath = 0xC3,
};

namespace network::response {

extern std::string getName(ResponseId id);

}
