#include <unordered_map>
#include "network/response/id.hpp"

namespace {

const std::unordered_map<ResponseId, std::string> response_names{
    {ResponseId::Ok, "OK"},
    {ResponseId::UnknownRequest, "UNKNOWN_REQUEST"},
    {ResponseId::UnknownUsername, "UNKNOWN_USERNAME"},
    {ResponseId::InvalidPassword, "INVALID_PASSWORD"},
    {ResponseId::InvalidData, "INVALID_DATA"},
    {ResponseId::InvalidBufferSize, "INVALID_BUFFER_SIZE"},
    {ResponseId::NotLogged, "NOT_LOGGED"},
    {ResponseId::IOFileNotExists, "IO_FILE_NOT_EXISTS"},
    {ResponseId::IOFileNotReadable, "IO_FILE_NOT_READABLE"},
    {ResponseId::IONotDirectory, "IO_NOT_DIRECTORY"},
    {ResponseId::IONotFile, "IO_NOT_FILE"},
    {ResponseId::IOInvalidPath, "IO_INVALID_PATH"},
    {ResponseId::IONonAbsolutePath, "IO_NON_ABSOLUTE_PATH"},
};

}

std::string network::response::getName(ResponseId id) {
    return response_names.contains(id) ? response_names.at(id) : "UNKNOWN";
}
