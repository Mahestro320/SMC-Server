#pragma once

#include "system/beg.hpp"

#include <string>

enum class RequestId : uint8_t {
    Login = 0x00,
    Logout = 0x01,
    RunSyscmd = 0x02,
    GetUserRole = 0x40,
    IOFileExists = 0x80,
    IOGetDirContent = 0x81,
    IOGetFileType = 0x82,
    IOGetFileSize = 0x83,
    IOGetFileContent = 0x84,
    IOGetRealPath = 0x85,
    IOSetFileContent = 0x86,
    IOCreateFile = 0x87,
    IOCreateDirectory = 0x88,
};

namespace network::request {

extern std::string getName(RequestId id);

} // namespace network::request
