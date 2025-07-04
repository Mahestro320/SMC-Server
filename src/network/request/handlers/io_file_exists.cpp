#include "io/console.hpp"
#include "network.hpp"
#include "network/request/handlers/io_file_exists.hpp"

using boost::asio::ip::tcp;
namespace fs = std::filesystem;

void IOFileExistsRH::run() {
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

bool IOFileExistsRH::getPath(tcp::socket& socket) {
    console::out::inf("getting path");
    std::string path_str{};
    if (!network::readString(socket, path_str)) {
        return false;
    }
    const SFS& sfs{client->getSFS()};
    path = sfs.absoluteFromUserScope(fs::path{path_str});
    return true;
}

bool IOFileExistsRH::checkPath(tcp::socket& socket) const {
    const SFS& sfs{client->getSFS()};
    return network::sendResponse(socket, sfs.exists(path) ? ResponseId::Ok : ResponseId::IOFileNotExists);
}
