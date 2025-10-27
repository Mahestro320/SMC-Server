#pragma once

#include "system/beg.hpp"

#include <string>

enum class RequestId : uint8_t {
    GetVersion = 0x00,
    GetUserRole,
    Login = 0x40,
    Logout,
    RunSyscmd,
    IOFileExists = 0x80,
    IOGetDirContent,
    IOGetFileType,
    IOGetFileSize,
    IOGetFileContent,
    IOGetRealPath,
    IOSetFileContent,
    IOCreateFile,
    IODeleteFile,
    IOCreateDirectory,
};

namespace network::request {

extern std::string getName(RequestId id);

} // namespace network::request
