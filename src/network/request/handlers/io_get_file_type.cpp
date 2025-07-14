#include "io/console.hpp"
#include "io/file_type.hpp"
#include "network.hpp"
#include "network/request/handlers/io_get_file_type.hpp"

using boost::asio::ip::tcp;
namespace fs = std::filesystem;

void IOGetFileTypeRH::run() {
    tcp::socket& socket{client->getSocket()};
    if (!client->isLogged()) {
        console::out::err("the client must be logged to do this");
        network::sendResponse(socket, ResponseId::NotLogged);
        return;
    }
    if (!network::sendResponse(socket, ResponseId::Ok)) {
        return;
    }
    getPath(socket) && checkPath(socket);
}

bool IOGetFileTypeRH::getPath(tcp::socket& socket) {
    console::out::inf("getting path");
    std::string path_str{};
    if (!network::readString(socket, path_str)) {
        return false;
    }
    const SFS& sfs{client->getSFS()};
    path = sfs.absoluteFromUserScope(fs::path{path_str});
    return true;
}

bool IOGetFileTypeRH::checkPath(tcp::socket& socket) const {
    const SFS& sfs{client->getSFS()};
    return network::sendInt(socket, static_cast<uint8_t>(sfs.getFileType(path)));
}
