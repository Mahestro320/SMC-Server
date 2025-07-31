#include "io/console.hpp"
#include "network.hpp"
#include "network/client.hpp"
#include "network/request/handlers/io_get_file_size.hpp"

using boost::asio::ip::tcp;
namespace fs = std::filesystem;

void IOGetFileSizeRH::run() {
    tcp::socket& socket{client->getSocket()};
    if (!network::sendResponse(socket, ResponseId::Ok) || !readTargetPath(socket)) {
        return;
    }
    getFileSize();
    if (sendPathResponse(socket) && path_response == ResponseId::Ok) {
        sendFileSize(socket);
    }
}

bool IOGetFileSizeRH::readTargetPath(tcp::socket& socket) {
    console::out::inf("reading target path");
    std::string path_str{};
    if (!network::readString(socket, path_str)) {
        return false;
    }
    const SFS& sfs{client->getSFS()};
    path = sfs.absoluteFromUserScope(fs::path{path_str});
    return true;
}

void IOGetFileSizeRH::getFileSize() {
    const SFS& sfs{client->getSFS()};
    if (!sfs.exists(path)) {
        path_response = ResponseId::IOFileNotExists;
        return;
    }
    size = sfs.getFileSize(path);
    return;
}

bool IOGetFileSizeRH::sendPathResponse(tcp::socket& socket) const {
    return network::sendResponse(socket, path_response);
}

bool IOGetFileSizeRH::sendFileSize(tcp::socket& socket) const {
    console::out::inf("sending file size");
    return network::sendInt<uint64_t>(socket, size);
}
