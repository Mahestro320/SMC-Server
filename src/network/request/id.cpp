#include "network/request/id.hpp"

#include <unordered_map>

namespace {

const std::unordered_map<RequestId, std::string> request_names{
    {RequestId::GetVersion, "GET_VERSION"},
    {RequestId::GetUserRole, "GET_USER_ROLE"},
    {RequestId::Login, "LOGIN"},
    {RequestId::Logout, "LOGOUT"},
    {RequestId::RunSyscmd, "RUN_SYSCMD"},
    {RequestId::IOFileExists, "IO_FILE_EXISTS"},
    {RequestId::IOGetDirContent, "IO_GET_DIR_CONTENT"},
    {RequestId::IOGetFileType, "IO_GET_FILE_TYPE"},
    {RequestId::IOGetFileSize, "IO_GET_FILE_SIZE"},
    {RequestId::IOGetFileContent, "IO_GET_FILE_CONTENT"},
    {RequestId::IOGetRealPath, "IO_GET_REAL_PATH"},
    {RequestId::IOSetFileContent, "IO_SET_FILE_CONTENT"},
    {RequestId::IOCreateFile, "IO_CREATE_FILE"},
    {RequestId::IODeleteFile, "IO_DELETE_FILE"},
    {RequestId::IOCreateDirectory, "IO_CREATE_DIRECTORY"},
};

}

std::string network::request::getName(RequestId id) {
    return request_names.contains(id) ? request_names.at(id) : "UNKNOWN";
}
