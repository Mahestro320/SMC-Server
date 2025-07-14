#include <unordered_map>
#include "network/request/id.hpp"

namespace {

const std::unordered_map<RequestId, std::string> request_names{
    {RequestId::Login, "LOGIN"},
    {RequestId::Logout, "LOGOUT"},
    {RequestId::GetUserRole, "GET_USER_ROLE"},
    {RequestId::IOFileExists, "IO_FILE_EXISTS"},
    {RequestId::IOGetDirContent, "IO_GET_DIR_CONTENT"},
    {RequestId::IOGetFileType, "IO_GET_FILE_TYPE"},
    {RequestId::IOGetFileSize, "IO_GET_FILE_SIZE"},
    {RequestId::IOGetFileContent, "IO_GET_FILE_CONTENT"},
    {RequestId::IOGetRealPath, "IO_GET_REAL_PATH"},
};

}

std::string network::request::getName(RequestId id) {
    return request_names.contains(id) ? request_names.at(id) : "UNKNOWN";
}
