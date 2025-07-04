#pragma once

#include <string>

enum class RequestId : uint8_t {
    Login = 0x00,
    Logout = 0x01,
    GetUserRole = 0x40,
    GetDirContent = 0x41,
    IOFileExists = 0x80,
    IOGetFileType = 0x81,
    IOGetRealPath = 0x82,
};

namespace network::request {

extern std::string getName(RequestId id);

} // namespace network::request
