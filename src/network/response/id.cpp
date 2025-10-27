#include "network/response/id.hpp"

#include <unordered_map>

namespace {

const std::unordered_map<ResponseId, std::string> response_names{
    {ResponseId::Ok, "OK"},
    {ResponseId::UnknownRequest, "UNKNOWN_REQUEST"},
    {ResponseId::UnknownUsername, "UNKNOWN_USERNAME"},
    {ResponseId::InvalidPassword, "INVALID_PASSWORD"},
    {ResponseId::InvalidData, "INVALID_DATA"},
    {ResponseId::InvalidBufferSize, "INVALID_BUFFER_SIZE"},
    {ResponseId::InvalidPermissions, "INVALID_PERMISSIONS"},
    {ResponseId::NotLogged, "NOT_LOGGED"},
    {ResponseId::IOError, "IO_ERROR"},
    {ResponseId::IOFileNotExists, "IO_FILE_NOT_EXISTS"},
    {ResponseId::IOFileNotReadable, "IO_FILE_NOT_READABLE"},
    {ResponseId::IONotDirectory, "IO_NOT_DIRECTORY"},
    {ResponseId::IONotFile, "IO_NOT_FILE"},
    {ResponseId::IOInvalidPath, "IO_INVALID_PATH"},
    {ResponseId::IONonAbsolutePath, "IO_NON_ABSOLUTE_PATH"},
    {ResponseId::IOAlreadyExists, "IO_ALREADY_EXISTS"},
};

}

std::string network::response::getName(ResponseId id) {
    return response_names.contains(id) ? response_names.at(id) : "UNKNOWN";
}
